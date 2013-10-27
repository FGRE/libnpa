#ifndef MAP_FILE_HPP
#define MAP_FILE_HPP

#include "scriptfile.hpp"

class MapFile : public ScriptFile
{
public:
    MapFile(const std::string& Name, char* Data, uint32_t Size);

private:
    virtual void Read(std::istream* pStream);
};

#endif
