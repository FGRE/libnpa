#include "npaiterator.hpp"
#include "npafile.hpp"

#include <cstring>
#include <boost/filesystem.hpp>

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

void NpaIterator::Save()
{
    Save(std::string());
}

void NpaIterator::Save(std::string Path)
{
    // Create directories
    std::string DPath = Path + GetFileName();
    char* delim = strchr((char*)DPath.c_str(), '/');
    do
    {
        *delim = 0;
        if (!boost::filesystem::exists(boost::filesystem::path(DPath)))
            boost::filesystem::create_directory(boost::filesystem::path(DPath));
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
