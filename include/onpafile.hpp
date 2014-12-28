#ifndef O_NPA_FILE_HPP
#define O_NPA_FILE_HPP

#include "npafile.hpp"
#include <map>

class ONpaFile : NpaFile
{
    struct Data
    {
        char* pData;
        uint32_t Size;
        uint32_t Offset;
    };
public:
    ONpaFile(const string& Filename);
    ~ONpaFile();

    void WriteFile(const string& Filename);
    void WriteFile(const string& Filename, char* pData, uint32_t Size);
private:
    void WriteToDisk();

    map<string, Data> Registry;
};

#endif
