#include "impkfile.hpp"
#include "fscommon.hpp"
#include "buffer.hpp"
#include <fstream>
#include <boost/algorithm/string/replace.hpp>
#include <zlib.h>

IMpkFile::IMpkFile(const string& Name) : INpaFile(Name)
{
    ReadHeader();
}

IMpkFile::~IMpkFile()
{
}

void IMpkFile::ReadHeader()
{
    uint32_t Size;
    char* pData = fs::ReadFile(Name, Size);
    if (!pData)
        return;

    Npa::Buffer File(pData, Size);
    File.Read(MPKHeader.Magic, 6);
    assert(strncmp("MPK\x00\x00\x00", MPKHeader.Magic, 6) == 0);

    MPKHeader.Version = File.Read<uint16_t>();
    MPKHeader.FileCount = File.Read<uint32_t>(); 
    File.Read(MPKHeader.Pad, 0x34);

    char NameBuf[224];

    for (uint32_t i = 0; i < MPKHeader.FileCount; ++i)
    {
        MpkEntry* MPKEntry = new MpkEntry;
        MPKEntry->Unk = File.Read<uint32_t>();
        MPKEntry->FileID = File.Read<uint32_t>();
        MPKEntry->Offset = File.Read<uint64_t>();
        MPKEntry->Size = File.Read<uint64_t>();
        uint64_t Size2 = File.Read<uint64_t>(); // ?
        assert(("The second size has a hidden meaning?", MPKEntry->Size == Size2));
        File.Read(NameBuf, 224);
        MPKEntry->Filename = string(NameBuf);

        string UtfPath = NpaFile::ToUtf8(MPKEntry->Filename);
        boost::replace_all(UtfPath, "\\", "/");
        Registry[UtfPath] = MPKEntry;
    }
}


char* IMpkFile::ReadFile(NpaIterator iter)
{
    return ReadData(iter, 0, ((MpkEntry*)iter->second)->Size);
}

char* IMpkFile::ReadData(NpaIterator iter, uint32_t LocalOffset, uint32_t Size, void *(*Alloc)(size_t))
{
    ifstream File(Name, ios::binary);
    if (!File)
        return nullptr;

    MpkEntry* MPKEntry = (MpkEntry*)iter->second;
    uint8_t* buffer = new uint8_t[Size];

    File.seekg(MPKEntry->Offset + LocalOffset);
    File.read((char*)buffer, Size);

    return (char*)buffer;
}

bool IMpkFile::IsDirectory(NpaIterator iter)
{
    return false;
}
