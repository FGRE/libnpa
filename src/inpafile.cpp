#include "inpafile.hpp"

INpaFile::~INpaFile()
{
}

char* INpaFile::ReadFile(const std::string& Filename, uint32_t& Size)
{
    auto iter = FindFile(Filename);
    if (iter == End())
        return nullptr;

    char* pData = ReadFile(iter);
    Size = GetFileSize(iter);
    return pData;
}

char* INpaFile::ReadFile(NpaIterator iter)
{
    return ReadFile(iter, 0, GetFileSize(iter));
}

char* INpaFile::ReadFile(NpaIterator iter, uint32_t Offset, uint32_t Size, void *(*Alloc)(size_t))
{
    return ReadData(iter->second->Offset, Offset, Size, Alloc);
}
