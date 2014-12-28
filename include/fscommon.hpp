#ifndef FS_COMMON_HPP
#define FS_COMMON_HPP

#include <string>
#include <cstdint>
using namespace std;

namespace fs
{
    char* ReadFile(const string& Filename, uint32_t& Size);
    void WriteFileDirectory(const string& Filename, const char* pData, uint32_t Size);
    void WriteFile(const string& Filename, const char* pData, uint32_t Size);
    bool Exists(const string& Path);
    void CreateDirectory(const string& Path);
    void DeleteFile(const string& Path);
    void DeleteDirectory(const string& Path);
}

#endif
