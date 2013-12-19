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
    NpaIterator();

    NpaIterator& operator++();
    NpaIterator operator++(int);
    bool operator==(const NpaIterator& Other);
    bool operator!=(const NpaIterator& Other);
    operator bool() { return File != nullptr && Pos != nullptr; }

    std::string GetFileName();
    char* GetFileNameRaw();
    char* GetFileData();
    char* GetFileData(uint32_t Offset, uint32_t Size);
    uint32_t GetFileSize();
    uint32_t GetOffset();
    uint32_t GetFileNameSize();

    void Remove();

private:
    void SetOffset(uint32_t offset);
    char* GetRawEntry();
    uint32_t GetRawEntrySize();

    char* Pos;
    NpaFile* File;
};

#endif
