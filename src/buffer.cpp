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

void Buffer::Write(const std::string& String)
{
    Write32(String.size());
    Write(String.c_str(), String.size());
}

void Buffer::Write16(int16_t Integer)
{
    Write(&Integer, sizeof(int16_t));
}

void Buffer::Write32(int32_t Integer)
{
    Write(&Integer, sizeof(int32_t));
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

std::string Buffer::Read()
{
    std::string String;
    String.resize(Read32());
    Read(&String[0], String.size());
    return String;
}

int16_t Buffer::Read16()
{
    int16_t Integer;
    Read(&Integer, sizeof(int16_t));
    return Integer;
}

int32_t Buffer::Read32()
{
    int32_t Integer;
    Read(&Integer, sizeof(int32_t));
    return Integer;
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

}
