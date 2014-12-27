#ifndef BUFFER_HPP
#define BUFFER_HPP

#include <vector>
#include <string>

namespace Npa
{

class Buffer
{
public:
    Buffer();
    Buffer(char* pData, uint32_t Size);
    ~Buffer();

    void Write(const std::string& String);
    void Write16(int16_t Integer);
    void Write32(int32_t Integer);
    void Write(const void* pData, uint32_t Size);
    void Write(const void* pData, uint32_t Size, uint32_t Offset);

    std::string Read();
    int16_t Read16();
    int32_t Read32();
    void Read(void* pDest, uint32_t Size);
    void Read(void* pDest, uint32_t Size, uint32_t Offset);

    char* GetData();
    uint32_t GetSize();
    uint32_t GetIter();
private:
    uint32_t Size, Iter;
    char* pData;
};

}
#endif
