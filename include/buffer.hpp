#ifndef BUFFER_HPP
#define BUFFER_HPP

#include <vector>
#include <string>
using namespace std;

namespace Npa
{

class Buffer
{
public:
    Buffer();
    Buffer(char* pData, uint32_t Size);
    ~Buffer();

    template <class T> void Write(T Integer)
    {
        Write(&Integer, sizeof(T));
    }
    void WriteStr16(const string& String);
    void WriteStr32(const string& String);
    void Write(const void* pData, uint32_t Size);
    void Write(const void* pData, uint32_t Size, uint32_t Offset);

    template <class T> T Read()
    {
        T Integer;
        Read(&Integer, sizeof(T));
        return Integer;
    }
    string ReadStr16();
    string ReadStr32();
    void Read(void* pDest, uint32_t Size);
    void Read(void* pDest, uint32_t Size, uint32_t Offset);

    char* GetData();
    uint32_t GetSize();
    uint32_t GetIter();
    bool EndOfBuffer();
private:
    uint32_t Size, Iter;
    char* pData;
};

}
#endif
