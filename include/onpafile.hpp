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
    ONpaFile(const std::string& Filename);
    ~ONpaFile();

    void WriteFile(const std::string& Filename);
    void WriteFile(const std::string& Filename, char* pData, uint32_t Size);
private:
    void WriteToDisk();

    std::map<std::string, Data> Registry;
};

#endif
