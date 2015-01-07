#ifndef O_NPA_FILE_HPP
#define O_NPA_FILE_HPP

#include "npafile.hpp"
#include <map>

class ONpaFile : NpaFile
{
public:
    ONpaFile(const string& Filename);
    virtual ~ONpaFile();

    void WriteFile(const string& Filename);

private:
    void WriteToDisk();
    map<string, uint32_t> Registry;
};

#endif
