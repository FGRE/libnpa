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

    char* ReadFileOffset(const std::string& Filename, uint32_t Size, uint32_t Offset);
    char* ReadFile(const std::string& Filename, uint32_t& Size);
    char* ReadFile(NpaIterator iter, uint32_t Offset = 0);
    NpaIterator Begin() { return Registry.begin(); }
    NpaIterator End() { return Registry.end(); }
    NpaIterator FindFile(const std::string& Filename) { return Registry.find(Filename); }
private:
    void ReadHeader();
    char* ReadData(uint32_t GlobalOffset, uint32_t LocalOffset, uint32_t Size);

    std::map<std::string, Entry> Registry;
};

#endif
