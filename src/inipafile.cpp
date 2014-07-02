#include "inipafile.hpp"
#include <fstream>
#include <boost/algorithm/string/replace.hpp>
#include <zlib.h>

extern uint8_t NpaKeyTable[][0x100];
typedef NipaEntry Entry;

INipaFile::INipaFile(const std::string& Name, uint8_t GameID) : INpaFile(Name), GameID(GameID)
{
    ReadHeader();
}

INipaFile::~INipaFile()
{
}

void INipaFile::ReadHeader()
{
    std::ifstream File(Name, std::ios::binary);
    if (!File)
        return;

    File.read(NPAHeader.Magic, 7);
    if (strncmp("NPA\x01", NPAHeader.Magic, 4) != 0)
        return;

    File.read((char*)&NPAHeader.Key1, 4);
    File.read((char*)&NPAHeader.Key2, 4);
    File.read((char*)&NPAHeader.Compress, 1);
    File.read((char*)&NPAHeader.Encrypt, 1);
    File.read((char*)&NPAHeader.TotalCount, 4);
    File.read((char*)&NPAHeader.FolderCount, 4);
    File.read((char*)&NPAHeader.FileCount, 4);
    File.read((char*)&NPAHeader.Unk, 8);
    File.read((char*)&NPAHeader.Start, 4);

    for (int i = 0; i < NPAHeader.TotalCount; ++i)
    {
        Entry NPAEntry;
        File.read((char*)&NPAEntry.NameSize, 4);

        NPAEntry.Filename = new char[NPAEntry.NameSize + 1];
        File.read((char*)NPAEntry.Filename, NPAEntry.NameSize);

        for (int x = 0; x < NPAEntry.NameSize; ++x)
            NPAEntry.Filename[x] += Crypt(x, i);
        NPAEntry.Filename[NPAEntry.NameSize] = '\0';

        File.read((char*)&NPAEntry.Type, 1);
        File.read((char*)&NPAEntry.FileID, 4);
        File.read((char*)&NPAEntry.Offset, 4);
        File.read((char*)&NPAEntry.compsize, 4);
        File.read((char*)&NPAEntry.origsize, 4);

        std::string UtfPath = NpaFile::ToUtf8(NPAEntry.Filename);
        boost::replace_all(UtfPath, "\\", "/");
        Registry[UtfPath] = NPAEntry;
    }
}

int INipaFile::Crypt(int32_t curnum, int32_t curfile)
{
    int key = 0xFC * curnum;
    int temp = 0;

    if (NPAHeader.Encrypt && (GameID == LAMENTO || GameID == LAMENTOTR))
        temp = NPAHeader.Key1 + NPAHeader.Key2;
    else
        temp = NPAHeader.Key1 * NPAHeader.Key2;

    key -= temp >> 0x18;
    key -= temp >> 0x10;
    key -= temp >> 0x08;
    key -= temp  & 0xff;
    key -= curfile >> 0x18;
    key -= curfile >> 0x10;
    key -= curfile >> 0x08;
    key -= curfile;
    return key & 0xff;
}

int INipaFile::Crypt2(char* name, int32_t origsize)
{
    int i = 0;
    int key1 = 0; /* 2345678 hurr */
    int key2 = NPAHeader.Key1 * NPAHeader.Key2;
    int key = 0;

    switch (GameID)
    {
        case AXANAEL:
        case KIKOKUGAI:
        case SONICOMITR2:
        case SONICOMI:
        case LOSTX:
        case DRAMATICALMURDER:
            key1 = 0x20101118;
            break;
        default:
            key1 = 0x87654321;
            break;
    }

    for (i = 0; name[i] != 0; ++i)
        key1 -= name[i];

    key = key1 * i;
    if (GameID != LAMENTO && GameID != LAMENTOTR)
    {
        key += key2;
        key *= origsize;
    }
    return key & 0xff;
}

char* INipaFile::ReadFile(NpaIterator iter, uint32_t Offset, uint32_t Size, void *(*Alloc)(size_t) = DefaultAlloc)
{
    return nullptr;
}

char* INipaFile::ReadFile(NpaIterator iter)
{
    std::ifstream File(Name, std::ios::binary);
    if (!File)
        return nullptr;

    Entry NPAEntry = iter->second;
    uint8_t* buffer = new uint8_t[NPAEntry.compsize];

    File.seekg(NPAEntry.Offset + NPAHeader.Start + 0x29);
    File.read((char*)buffer, NPAEntry.compsize);

    if (NPAHeader.Encrypt == 1)
    {
        int key = Crypt2(NPAEntry.Filename, NPAEntry.origsize);
        int len = 0x1000;

        if (GameID != LAMENTO && GameID != LAMENTOTR)
            len += strlen(NPAEntry.Filename);

        for (int x = 0; x < NPAEntry.compsize && x < len; ++x)
        {
            if (GameID == LAMENTO || GameID == LAMENTOTR)
                buffer[x] = NpaKeyTable[GameID][buffer[x]] - key;
            else
                buffer[x] = (NpaKeyTable[GameID][buffer[x]] - key) - x;
        }
    }

    if (NPAHeader.Compress == 1)
    {
        char* zbuffer = new char[NPAEntry.origsize];
        uLongf sz = NPAEntry.origsize;
        assert(uncompress((Bytef*)zbuffer, &sz, buffer, NPAEntry.compsize) == Z_OK);
        delete[] buffer;
        return zbuffer;
    }
    return (char*)buffer;
}

char* INipaFile::ReadFile(const std::string& Filename, uint32_t& Size)
{
    auto iter = FindFile(Filename);
    if (iter == End())
        return nullptr;

    char* pData = ReadFile(iter);
    Size = iter->second.origsize;
    return pData;
}
