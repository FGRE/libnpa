#include "scriptfile.hpp"
#include "npafile.hpp"
#include "fscommon.hpp"
#include "buffer.hpp"
#include <cstring>
using namespace Npa;

ScriptFile::ScriptFile(const std::string& Name, const char* NssData, uint32_t NssSize) :
Name(Name)
{
    ReadNss(NssData, NssSize);
}

ScriptFile::ScriptFile(const std::string& Name, char* NsbData, uint32_t NsbSize, char* MapData, uint32_t MapSize) :
Name(Name)
{
    Buffer NsbBuffer(NsbData, NsbSize);
    Buffer MapBuffer(MapData, MapSize);
    ReadNsb(NsbBuffer, MapBuffer);
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
    Buffer NsbBuffer(NsbData, NsbSize);
    Buffer MapBuffer(MapData, MapSize);
    ReadNsb(NsbBuffer, MapBuffer);
}

void ScriptFile::ReadNss(const char* NssData, uint32_t NssSize)
{
    Buffer NsbBuffer, MapBuffer;
    Nss::Compile(NssData, NssSize, &NsbBuffer, &MapBuffer);
    ReadNsb(NsbBuffer, MapBuffer);
}

void ScriptFile::ReadNsb(Npa::Buffer& NsbData, Npa::Buffer& MapData)
{
    uint32_t Entry;
    uint16_t NumParams;
    Line* CurrLine;

    // Read source code lines
    while (NsbData.GetIter() < NsbData.GetSize())
    {
        Entry = NsbData.Read32() - 1; // Start counting at zero
        Source.resize(Source.size() + 1);
        CurrLine = &Source[Entry];
        CurrLine->Magic = NsbData.Read16();
        NumParams = NsbData.Read16();
        CurrLine->Params.reserve(NumParams);

        // Read parameters
        for (uint16_t i = 0; i < NumParams; ++i)
            CurrLine->Params.push_back(NpaFile::ToUtf8(NsbData.Read()));
    }

    uint32_t Offset;
    uint16_t Size;
    std::string Label;

    // Read symbols
    while (MapData.GetIter() < MapData.GetSize())
    {
        Offset = MapData.Read32();
        Size = MapData.Read16();
        Label.resize(Size);
        MapData.Read(&Label[0], Size);
        NsbData.Read(&Entry, sizeof(uint32_t), Offset);

        Label = NpaFile::ToUtf8(Label);
        if (Label.substr(0, 7) == "include")
            Includes.push_back(Label.substr(8));
        else
            Symbols[Label] = Entry - 1;
    }
}

uint32_t ScriptFile::GetSymbol(const std::string& Symbol)
{
    auto iter = Symbols.find(Symbol);
    if (iter != Symbols.end())
        return iter->second;
    return NSB_INVALIDE_LINE;
}
