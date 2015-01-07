#include "onpafile.hpp"
#include "fscommon.hpp"
#include "buffer.hpp"
#include <fstream>
using namespace Npa;

ONpaFile::ONpaFile(const string& Filename) : NpaFile(Filename)
{
}

ONpaFile::~ONpaFile()
{
    WriteToDisk();
}

void ONpaFile::WriteFile(const string& Filename)
{
    Registry[Filename] = 0;
}

void ONpaFile::WriteToDisk()
{
    // Create header
    Buffer Header;
    Header.Write<uint32_t>(Registry.size());
    for (auto iter = Registry.begin(); iter != Registry.end(); ++iter)
    {
        Header.WriteStr32(FromUtf8(iter->first));
        Header.Write<uint32_t>(fs::FileSize(iter->first));
        iter->second = Header.GetSize();
        Header.Write<uint32_t>(0); // Offset
        Header.Write<uint32_t>(0); // Unk
    }

    // Calculate offsets
    uint32_t Pos = 4 + Header.GetSize();
    for (auto iter = Registry.begin(); iter != Registry.end(); ++iter)
    {
        Header.Write(&Pos, 4, iter->second);
        Pos += fs::FileSize(iter->first);
    }

    // Write data
    ofstream File(Name, ios::binary);
    uint32_t Size = Header.GetSize();
    File.write((char*)&Size, 4);
    File.write(Encrypt(Header.GetData(), Size), Size);
    for (auto iter = Registry.begin(); iter != Registry.end(); ++iter)
    {
        uint32_t Size;
        char* pData = fs::ReadFile(iter->first, Size);
        File.write(Encrypt(pData, Size), Size);
        delete pData;
    }
}
