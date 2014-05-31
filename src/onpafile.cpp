#include "onpafile.hpp"
#include "fscommon.hpp"
#include <fstream>

struct Buffer
{
    uint32_t Size()
    {
        return Data.size();
    }

    void Write(const std::string& String)
    {
        Write(String.size());
        Write(String.c_str(), String.size());
    }

    void Write(uint32_t Integer)
    {
        Write(&Integer, 4);
    }

    void Write(const void* pData, uint32_t Size)
    {
        Data.resize(this->Size() + Size);
        Write(pData, Size, this->Size() - Size);
    }

    void Write(const void* pData, uint32_t Size, uint32_t Offset)
    {
        std::memcpy(&Data[Offset], pData, Size);
    }

    std::vector<char> Data;
};

ONpaFile::ONpaFile(const std::string& Filename) : NpaFile(Filename)
{
}

ONpaFile::~ONpaFile()
{
    WriteToDisk();
}

void ONpaFile::WriteFile(const std::string& Filename)
{
    uint32_t Size;
    char* pData = fs::ReadFile(Filename, Size);
    WriteFile(Filename, pData, Size);
}

void ONpaFile::WriteFile(const std::string& Filename, char* pData, uint32_t Size)
{
    Registry[Filename] = { pData, Size, 0 };
}

void ONpaFile::WriteToDisk()
{
    // Create header
    Buffer Header;
    Header.Write(Registry.size());
    for (auto iter = Registry.begin(); iter != Registry.end(); ++iter)
    {
        Header.Write(FromUtf8(iter->first));
        Header.Write(iter->second.Size);
        iter->second.Offset = Header.Size();
        Header.Write(0); // Offset
        Header.Write(0); // Unk
    }

    // Calculate offsets
    uint32_t Pos = 4 + Header.Size();
    for (auto iter = Registry.begin(); iter != Registry.end(); ++iter)
    {
        Header.Write(&Pos, 4, iter->second.Offset);
        Pos += iter->second.Size;
    }

    // Write data
    std::ofstream File(Name, std::ios::binary);
    uint32_t Size = Header.Size();
    File.write((char*)&Size, 4);
    File.write(Encrypt(&Header.Data[0], Size), Size);
    for (auto iter = Registry.begin(); iter != Registry.end(); ++iter)
    {
        File.write(Encrypt(iter->second.pData, iter->second.Size), iter->second.Size);
        delete iter->second.pData;
    }
}
