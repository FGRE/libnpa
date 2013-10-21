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
    uint32_t GetOffset();

    void Save();
    void Save(std::string Path);
    void Remove();

private:
    uint32_t GetFileNameSize();
    void SetOffset(uint32_t offset);
    char* GetRawEntry();
    uint32_t GetRawEntrySize();

    char* FileData;
    char* Pos;
    NpaFile* File;
};

#endif
