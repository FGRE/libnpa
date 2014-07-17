#include "buffer.hpp"
#include <cstring>

namespace NpaPrivate
{

uint32_t Buffer::Size()
{
    return Data.size();
}

void Buffer::Write(const std::string& String)
{
    Write32(String.size());
    Write(String.c_str(), String.size());
}

void Buffer::Write16(uint16_t Integer)
{
    Write(&Integer, sizeof(uint16_t));
}

void Buffer::Write32(uint32_t Integer)
{
    Write(&Integer, sizeof(uint32_t));
}

void Buffer::Write(const void* pData, uint32_t Size)
{
    Data.resize(this->Size() + Size);
    Write(pData, Size, this->Size() - Size);
}

void Buffer::Write(const void* pData, uint32_t Size, uint32_t Offset)
{
    std::memcpy(&Data[Offset], pData, Size);
}

}
