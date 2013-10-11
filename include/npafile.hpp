#ifndef NPA_FILE_HPP
#define NPA_FILE_HPP

#include "npaiterator.hpp"

#include <fstream>
#include <cstdint>

class NpaFile
{
    friend class NpaIterator;
public:
    NpaFile(std::string Name);
    ~NpaFile();

    void Open(std::string Name);
    void Close();

    NpaIterator Begin();
    NpaIterator End();

private:
    void ReadEncrypted(char* buff, uint32_t offset, uint32_t size);

    char* Header;
    uint32_t HeaderSize;
    std::fstream File;
};

#endif
