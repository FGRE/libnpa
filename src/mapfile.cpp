#include "mapfile.hpp"

MapFile::MapFile(const std::string& Name, char* Data, uint32_t Size)
{
    Open(Name, Data, Size);
}

void MapFile::Read(std::istream* pStream)
{
}
