#include "scriptfile.hpp"
#include "npafile.hpp"

#include <cstring>
#include <fstream>

ScriptFile::ScriptFile(std::string Name, char* NsbData, uint32_t NsbSize, char* MapData, uint32_t MapSize) :
Name(Name)
{
    Open(NsbData, NsbSize, MapData, MapSize);
}

ScriptFile::ScriptFile(std::string Name) :
Name(Name)
{
    std::ifstream NsbFile(Name, std::ios::binary);
    NsbFile.seekg(0, std::ios::end);
    uint32_t NsbSize = NsbFile.tellg();
    char* NsbData = new char[NsbSize];
    NsbFile.seekg(0, std::ios::beg);
    NsbFile.read(NsbData, NsbSize);

    std::string MapName = std::string(Name, 0, Name.size() - 3) + "map";
    std::ifstream MapFile(MapName, std::ios::binary);
    MapFile.seekg(0, std::ios::end);
    uint32_t MapSize = MapFile.tellg();
    char* MapData = new char[MapSize];
    MapFile.seekg(0, std::ios::beg);
    MapFile.read(MapData, MapSize);

    Open(NsbData, NsbSize, MapData, MapSize);

    delete[] NsbData;
    delete[] MapData;
}

void ScriptFile::Open(char* NsbData, uint32_t NsbSize, char* MapData, uint32_t MapSize)
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
        Symbols[Label] = Entry - 1;
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
