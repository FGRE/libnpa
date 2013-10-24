#include "npaiterator.hpp"
#include "npafile.hpp"

#include <cstring>

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
    delete[] FileData;
    FileData = nullptr;

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
    std::memcpy(&Ret[0], Pos + sizeof(uint32_t), Ret.size());
    return Ret;
}

char* NpaIterator::GetFileNameRaw()
{
    return Pos + sizeof(uint32_t);
}

uint32_t NpaIterator::GetFileSize()
{
    return *(uint32_t*)(Pos + sizeof(uint32_t) + GetFileNameSize());
}

char* NpaIterator::GetFileData()
{
    if (FileData)
        return FileData;

    FileData = new char[GetFileSize()];
    File->ReadEncrypted(FileData, GetOffset(), GetFileSize());
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
