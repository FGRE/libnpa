#ifndef NPA_FILE_HPP
#define NPA_FILE_HPP

#include "npaiterator.hpp"

#include <fstream>
#include <cstdint>
#include <vector>

enum OpenMode
{
    NPA_READ   = 1,
    NPA_CREATE = 2
};

class NpaFile
{
    friend class NpaIterator;
public:
    NpaFile(std::string _Name, OpenMode Mode);
    ~NpaFile();

    void Close();

    NpaIterator Begin();
    NpaIterator End();

    void AppendFile(std::string Name, const char* buff, uint32_t size);
    void AppendFile(std::string Name);

private:
    void ReadHeader(std::string& Name);

    void Flush();
    void ReadEncrypted(char* buff, uint32_t offset, uint32_t size);
    char* XOR(char* buff, uint32_t size, uint32_t keyoff);

    char* Header;
    std::vector<char> NewHeader;
    std::vector<char> NewData;
    uint32_t HeaderSize;
    uint32_t EntryCount;
    std::fstream File;
    std::string Name;
};

#endif
