#include "buffer.hpp"
#include <cstring>

namespace Npa
{

Buffer::Buffer() : pData(nullptr), Size(0), Iter(0)
{
}

Buffer::Buffer(char* pData, uint32_t Size) : pData(pData), Size(Size), Iter(0)
{
}

Buffer::~Buffer()
{
    delete[] pData;
}

void Buffer::WriteStr16(const std::string& String)
{
    Write<uint16_t>(String.size());
    Write(String.c_str(), String.size());
}

void Buffer::WriteStr32(const std::string& String)
{
    Write<uint32_t>(String.size());
    Write(String.c_str(), String.size());
}

void Buffer::Write(const void* pData, uint32_t Size)
{
    char* pNew = new char[this->Size + Size];
    std::memcpy(pNew, this->pData, this->Size);
    delete[] this->pData;
    this->pData = pNew;
    Write(pData, Size, this->Size);
    this->Size += Size;
}

void Buffer::Write(const void* pData, uint32_t Size, uint32_t Offset)
{
    std::memcpy(this->pData + Offset, pData, Size);
}

std::string Buffer::ReadStr16()
{
    std::string String;
    String.resize(Read<uint16_t>());
    Read(&String[0], String.size());
    return String;
}

std::string Buffer::ReadStr32()
{
    std::string String;
    String.resize(Read<uint32_t>());
    Read(&String[0], String.size());
    return String;
}

void Buffer::Read(void* pDest, uint32_t Size)
{
    Read(pDest, Size, Iter);
    Iter += Size;
}

void Buffer::Read(void* pDest, uint32_t Size, uint32_t Offset)
{
    std::memcpy(pDest, pData + Offset, Size);
}

char* Buffer::GetData()
{
    return pData;
}

uint32_t Buffer::GetSize()
{
    return Size;
}

uint32_t Buffer::GetIter()
{
    return Iter;
}

bool Buffer::EndOfBuffer()
{
    return Iter >= Size;
}

}
