#ifndef BUFFER_HPP
#define BUFFER_HPP

#include <vector>
#include <string>

namespace NpaPrivate
{

struct Buffer
{
    void Write(const std::string& String);
    void Write16(uint16_t Integer);
    void Write32(uint32_t Integer);
    void Write(const void* pData, uint32_t Size);
    void Write(const void* pData, uint32_t Size, uint32_t Offset);

    uint32_t Size();
    std::vector<char> Data;
};

}
#endif
