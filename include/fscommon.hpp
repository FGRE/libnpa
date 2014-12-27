#ifndef FS_COMMON_HPP
#define FS_COMMON_HPP

#include <string>
#include <cstdint>

namespace fs
{
    char* ReadFile(const std::string& Filename, uint32_t& Size);
    void WriteFileDirectory(const std::string& Filename, const char* pData, uint32_t Size);
    void WriteFile(const std::string& Filename, const char* pData, uint32_t Size);
    bool Exists(const std::string& Path);
    void CreateDirectory(const std::string& Path);
    void DeleteFile(const std::string& Path);
    void DeleteDirectory(const std::string& Path);
}

#endif
