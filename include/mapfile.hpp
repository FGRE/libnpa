#ifndef MAP_FILE_HPP
#define MAP_FILE_HPP

#include "scriptfile.hpp"

struct MapLine
{
    uint32_t Magic;
    std::string Value;
};

class MapFile : public ScriptFile<MapLine>
{
public:
    MapFile(const std::string& Name, char* Data = nullptr, uint32_t Size = 0);

private:
    virtual void Read(std::istream* pStream);
};

#endif
