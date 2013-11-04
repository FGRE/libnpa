#include "mapfile.hpp"

#include <fstream>

MapFile::MapFile(const std::string& Name, char* Data, uint32_t Size)
{
    Open(Name, Data, Size);
}

void MapFile::Read(std::istream* pStream)
{
    uint32_t Magic;
    uint16_t Size;
    std::string Value;

    while (pStream->read((char*)&Magic, sizeof(uint32_t)))
    {
        pStream->read((char*)&Size, sizeof(uint16_t));
        Value.resize(Size);
        pStream->read(&Value[0], Size);
        Source.push_back({Magic, Value});
    }
}
