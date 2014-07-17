#include "onpafile.hpp"
#include "fscommon.hpp"
#include "buffer.hpp"
#include <fstream>
using namespace NpaPrivate;

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
    Header.Write32(Registry.size());
    for (auto iter = Registry.begin(); iter != Registry.end(); ++iter)
    {
        Header.Write(FromUtf8(iter->first));
        Header.Write32(iter->second.Size);
        iter->second.Offset = Header.Size();
        Header.Write32(0); // Offset
        Header.Write32(0); // Unk
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
