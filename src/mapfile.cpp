#include "mapfile.hpp"

#include <fstream>

MapFile::MapFile(const std::string& Name, char* Data, uint32_t Size)
{
    Open(Name, Data, Size);
}

void MapFile::Read(std::istream* pStream)
{
    uint32_t Offset;
    uint16_t Size;
    std::string Label;

    while (pStream->read((char*)&Offset, sizeof(uint32_t)))
    {
        pStream->read((char*)&Size, sizeof(uint16_t));
        Label.resize(Size);
        pStream->read(&Label[0], Size);
        Source.push_back({Offset, Label});
    }
}
