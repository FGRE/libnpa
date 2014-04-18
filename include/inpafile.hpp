#ifndef I_NPA_FILE_HPP
#define I_NPA_FILE_HPP

#include "npafile.hpp"
#include <map>

class INpaFile : NpaFile
{
    struct Entry
    {
        uint32_t Size;
        uint32_t Offset;
        uint32_t Unk;
    };
public:
    INpaFile(const std::string& Filename);
    ~INpaFile();

    char* ReadFile(const std::string& Filename, uint32_t& Size);
private:
    void ReadHeader();

    std::map<std::string, Entry> Registry;
};

#endif
