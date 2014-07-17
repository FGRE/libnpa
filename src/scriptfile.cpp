#include "scriptfile.hpp"
#include "npafile.hpp"
#include "fscommon.hpp"
#include "buffer.hpp"
#include <cstring>
using namespace NpaPrivate;

ScriptFile::ScriptFile(const std::string& Name, char* NssData, uint32_t NssSize) :
Name(Name)
{
    ReadNss(NssData, NssSize);
}

ScriptFile::ScriptFile(const std::string& Name, char* NsbData, uint32_t NsbSize, char* MapData, uint32_t MapSize) :
Name(Name)
{
    ReadNsb(NsbData, NsbSize, MapData, MapSize);
}

ScriptFile::ScriptFile(const std::string& Name, FileType Type) :
Name(Name)
{
    switch (Type)
    {
        case NSS: OpenNss(Name); break;
        case NSB: OpenNsb(Name); break;
        default: assert(false);
    }
}

void ScriptFile::OpenNss(const std::string& Name)
{
    uint32_t NssSize;
    char* NssData = fs::ReadFile(Name, NssSize);
    ReadNss(NssData, NssSize);
    delete[] NssData;
}

void ScriptFile::OpenNsb(std::string Name)
{
    std::string MapName = std::string(Name, 0, Name.size() - 3) + "map";

    uint32_t NsbSize, MapSize;
    char* NsbData = fs::ReadFile(Name, NsbSize);
    char* MapData = fs::ReadFile(MapName, MapSize);
    ReadNsb(NsbData, NsbSize, MapData, MapSize);

    delete[] NsbData;
    delete[] MapData;
}

void ScriptFile::ReadNss(char* NssData, uint32_t NssSize)
{
    Buffer NsbBuffer, MapBuffer;
    Nss::Compile(NssData, NssSize, &NsbBuffer, &MapBuffer);
    ReadNsb(&NsbBuffer.Data[0], NsbBuffer.Size(), &MapBuffer.Data[0], MapBuffer.Size());
}

void ScriptFile::ReadNsb(char* NsbData, uint32_t NsbSize, char* MapData, uint32_t MapSize)
{
    uint32_t Entry, Length;
    uint16_t NumParams;
    Line* CurrLine;
    char* Iter;

    // Read source code lines
    Iter = NsbData;
    while (Iter < NsbData + NsbSize)
    {
        Read(&Iter, &Entry, sizeof(uint32_t));
        Entry -= 1; // Start counting at zero
        Source.resize(Source.size() + 1);
        CurrLine = &Source[Entry];
        Read(&Iter, &CurrLine->Magic, sizeof(uint16_t));
        Read(&Iter, &NumParams, sizeof(uint16_t));
        CurrLine->Params.reserve(NumParams);

        // Read parameters
        for (uint16_t i = 0; i < NumParams; ++i)
        {
            Read(&Iter, &Length, sizeof(uint32_t));
            char* String = new char[Length];
            Read(&Iter, String, Length);
            CurrLine->Params.push_back(NpaFile::ToUtf8(std::string(String, Length)));
            delete[] String;
        }
    }

    uint32_t Offset;
    uint16_t Size;
    std::string Label;

    // Read symbols
    Iter = MapData;
    while (Iter < MapData + MapSize)
    {
        Read(&Iter, &Offset, sizeof(uint32_t));
        Read(&Iter, &Size, sizeof(uint16_t));
        Label.resize(Size);
        Read(&Iter, &Label[0], Size);
        std::memcpy(&Entry, NsbData + Offset, sizeof(uint32_t));
        Symbols[NpaFile::ToUtf8(Label)] = Entry - 1;
    }
}

void ScriptFile::Read(char** Src, void* Dest, uint32_t Size)
{
    std::memcpy(Dest, *Src, Size);
    *Src += Size;
}

uint32_t ScriptFile::GetSymbol(const std::string& Symbol)
{
    auto iter = Symbols.find(Symbol);
    if (iter != Symbols.end())
        return iter->second;
    return NSB_INVALIDE_LINE;
}
