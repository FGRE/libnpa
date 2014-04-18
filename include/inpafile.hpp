#ifndef I_NPA_FILE_HPP
#define I_NPA_FILE_HPP

#include "npafile.hpp"
#include <map>

class INpaFile : NpaFile
{
public:
    struct Entry
    {
        uint32_t Size;
        uint32_t Offset;
        uint32_t Unk;
    };
    typedef std::map<std::string, Entry>::iterator NpaIterator;
public:
    INpaFile(const std::string& Filename);
    ~INpaFile();

    char* ReadFile(const std::string& Filename, uint32_t& Size);
    char* ReadFile(NpaIterator iter);
    NpaIterator Begin() { return Registry.begin(); }
    NpaIterator End() { return Registry.end(); }
private:
    void ReadHeader();

    std::map<std::string, Entry> Registry;
};

#endif
