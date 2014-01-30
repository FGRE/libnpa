#include "npaiterator.hpp"
#include "npafile.hpp"

#include <cstring>

/* PUBLIC */

NpaIterator::NpaIterator(NpaFile* File, char* Pos) :
Pos(Pos),
File(File)
{
}

NpaIterator::NpaIterator() :
Pos(nullptr),
File(nullptr)
{
}

NpaIterator& NpaIterator::operator++()
{
    Pos += GetRawEntrySize();

    if ((File && (Pos - File->Header >= File->HeaderSize)) || GetFileNameSize() == 0)
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
    std::memcpy(&Ret[0], GetFileNameRaw(), Ret.size());
    return Ret;
}

char* NpaIterator::GetFileNameRaw()
{
    return Pos + sizeof(uint32_t);
}

uint32_t NpaIterator::GetFileSize()
{
    return Pos ? *(uint32_t*)(Pos + sizeof(uint32_t) + GetFileNameSize()) : 0;
}

char* NpaIterator::GetFileData()
{
    return GetFileData(0, GetFileSize());
}

char* NpaIterator::GetFileData(uint32_t Offset, uint32_t Size)
{
    char* FileData = new char[Size];
    File->ReadEncrypted(FileData, GetOffset() + Offset, Size);
    return FileData;
}

void NpaIterator::Remove()
{
    // Set file size to 0 (See: GetFileSize())
    *(uint32_t*)(Pos + sizeof(uint32_t) + GetFileNameSize()) = 0;
    --File->EntryCount;
}

/* PRIVATE */

uint32_t NpaIterator::GetFileNameSize()
{
    return *(uint32_t*)Pos;
}

void NpaIterator::SetOffset(uint32_t offset)
{
    *(uint32_t*)(Pos + 2 * sizeof(uint32_t) + GetFileNameSize()) = offset;
}

uint32_t NpaIterator::GetOffset()
{
    return *(uint32_t*)(Pos + 2 * sizeof(uint32_t) + GetFileNameSize());
}

char* NpaIterator::GetRawEntry()
{
    return Pos;
}

uint32_t NpaIterator::GetRawEntrySize()
{
    return 4 * sizeof(uint32_t) + GetFileNameSize();
}
