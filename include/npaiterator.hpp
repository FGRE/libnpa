#ifndef NPA_ITERATOR_HPP
#define NPA_ITERATOR_HPP

#include <string>
#include <cstdint>

class NpaFile;

class NpaIterator
{
    friend class NpaFile;
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
    void Remove();

private:
    uint32_t GetFileNameSize();
    void SetOffset(uint32_t offset);
    uint32_t GetOffset();
    char* GetRawEntry();
    uint32_t GetRawEntrySize();

    char* FileData;
    char* Pos;
    NpaFile* File;
};

#endif
