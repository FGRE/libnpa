#include "npaiterator.hpp"
#include "npafile.hpp"

#include <cstring>
#include <unistd.h>
#include <sys/stat.h>

/* PUBLIC */

NpaIterator::NpaIterator(NpaFile* File, char* Pos) :
FileData(nullptr),
Pos(Pos),
File(File)
{
}

NpaIterator::~NpaIterator()
{
    delete[] FileData;
}

NpaIterator& NpaIterator::operator++()
{
    Pos += GetRawEntrySize();
    if (Pos - File->Header >= File->HeaderSize)
        Pos = nullptr;
    return *this;
}

NpaIterator NpaIterator::operator++(int)
{
    NpaIterator ret(File, Pos);
    ++(*this);
    return ret;
}

bool NpaIterator::operator==(const NpaIterator& Other)
{
    return Other.Pos == this->Pos;
}

bool NpaIterator::operator!=(const NpaIterator& Other)
{
    return !(*this == Other);
}

std::string NpaIterator::GetFileName()
{
    std::string Ret;
    Ret.resize(GetFileNameSize());
    std::memcpy(&Ret[0], Pos + sizeof(uint32_t), Ret.size());
    return Ret;
}

uint32_t NpaIterator::GetFileSize()
{
    char offset = sizeof(uint32_t) + GetFileNameSize();
    return *(uint32_t*)(Pos + offset);
}

char* NpaIterator::GetFileData()
{
    if (FileData)
        return FileData;

    char offset = 2 * sizeof(uint32_t) + GetFileNameSize();
    FileData = new char[GetFileSize()];
    File->ReadEncrypted(FileData, *(uint32_t*)(Pos + offset), GetFileSize());
    return FileData;
}

void NpaIterator::Remove()
{
    // Set file size to 0 (See: GetFileSize())
    char offset = sizeof(uint32_t) + GetFileNameSize();
    *(uint32_t*)(Pos + offset) = 0;
    --File->EntryCount;
}

void NpaIterator::WriteToDisk()
{
    WriteToDisk(std::string());
}

void NpaIterator::WriteToDisk(std::string Path)
{
    // Create directories
    std::string DPath = Path + GetFileName();
    char* delim = strchr((char*)DPath.c_str(), '/');
    do
    {
        *delim = 0;
        struct stat st = {0};
        if (stat(DPath.c_str(), &st) == -1)
            mkdir(DPath.c_str(), 0700);
        *delim = '/';
    } while ((delim = strchr(delim + 1, '/')));

    // Create file
    std::ofstream Out(GetFileName(), std::ios::binary | std::ios::out);
    Out.write(GetFileData(), GetFileSize());
}

/* PRIVATE */

uint32_t NpaIterator::GetFileNameSize()
{
    return *(uint32_t*)Pos;
}

void NpaIterator::SetOffset(uint32_t offset)
{
    char off = 2 * sizeof(uint32_t) + GetFileNameSize();
    *(uint32_t*)(Pos + off) = offset;
}

uint32_t NpaIterator::GetOffset()
{
    char off = 2 * sizeof(uint32_t) + GetFileNameSize();
    return *(uint32_t*)(Pos + off);
}

char* NpaIterator::GetRawEntry()
{
    return Pos;
}

uint32_t NpaIterator::GetRawEntrySize()
{
    return 4 * sizeof(uint32_t) + GetFileNameSize();
}
