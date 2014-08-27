#include "inpafile.hpp"

INpaFile::~INpaFile()
{
     for (auto i = Registry.begin(); i != Registry.end(); ++i)
        delete i->second;
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
