#include "inpafile.hpp"
#include <fstream>

void* DefaultAlloc(uint64_t Size)
{
    return new char[Size];
}

INpaFile::INpaFile(const std::string& Filename) : NpaFile(Filename)
{
    ReadHeader();
}

INpaFile::~INpaFile()
{
}

char* INpaFile::ReadFile(const std::string& Filename, uint32_t& Size)
{
    auto iter = FindFile(Filename);
    if (iter == End())
        return nullptr;

    char* pData = ReadFile(iter);
    Size = iter->second.Size;
    return pData;
}

char* INpaFile::ReadFile(NpaIterator iter)
{
    return ReadFile(iter, 0, iter->second.Size);
}

char* INpaFile::ReadFile(NpaIterator iter, uint32_t Offset, uint32_t Size, void *(*Alloc)(size_t))
{
    return ReadData(iter->second.Offset, Offset, Size, Alloc);
}

char* INpaFile::ReadData(uint32_t GlobalOffset, uint32_t LocalOffset, uint32_t Size, void *(*Alloc)(size_t))
{
    std::ifstream File(Name, std::ios::binary);
    if (!File)
        return nullptr;

    char* pData = (char*)Alloc(Size);
    File.seekg(GlobalOffset + LocalOffset, File.beg);
    File.read(pData, Size);
    Decrypt(pData, Size, LocalOffset);
    return pData;
}

void INpaFile::ReadHeader()
{
    std::ifstream File(Name, std::ios::binary);
    if (!File)
        return;

    uint32_t HeaderSize, EntryCount;
    File.read((char*)&HeaderSize, 4);
    char* pHeader = new char[HeaderSize];
    File.read(pHeader, HeaderSize);
    Decrypt(pHeader, HeaderSize);
    EntryCount = *(uint32_t*)pHeader;
    pHeader += 4;

    for (uint32_t i = 0; i < EntryCount; ++i)
    {
        uint32_t FilenameSize = *(uint32_t*)pHeader;
        pHeader += 4;
        std::string Filename(pHeader, FilenameSize);
        pHeader += FilenameSize;
        Registry[ToUtf8(Filename)] = *(Entry*)pHeader;
        pHeader += sizeof(Entry);
    }
}
