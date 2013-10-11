#ifndef NPA_ITERATOR_HPP
#define NPA_ITERATOR_HPP

#include <string>
#include <cstdint>

class NpaFile;

class NpaIterator
{
public:
    NpaIterator(NpaFile* File, char* Pos);
    ~NpaIterator();

    NpaIterator& operator++();
    NpaIterator operator++(int);
    bool operator==(const NpaIterator& Other);
    bool operator!=(const NpaIterator& Other);

    std::string GetFileName();
    char* GetFileData();
    uint32_t GetFileSize();

    void WriteToDisk();
    void WriteToDisk(std::string Path);

private:
    uint32_t GetFileNameSize();

    char* FileData;
    char* Pos;
    NpaFile* File;
};

#endif
