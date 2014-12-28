#include "inipafile.hpp"
#include <fstream>
#include <boost/algorithm/string/replace.hpp>
#include <zlib.h>

extern uint8_t NpaKeyTable[][0x100];

static const char* NipaGames[] =
{
    "ChaosHead",
    "ChaosHeadTR1",
    "ChaosHeadTR2",
    "MuramasaTR",
    "Muramasa",
    "Sumaga",
    "Django",
    "DjangoTR",
    "Lamento",
    "LamentoTR",
    "SweetPool",
    "SumagaSP",
    "Demonbane",
    "MuramasaAD",
    "Axanel",
    "Kikokugai",
    "SoniComiTR2",
    "Sumaga3P",
    "SoniComi",
    "LostX",
    "LostXTrailer",
    "DramaticalMurder",
    "Totono",
    nullptr
};

const char** INipaFile::GetGameStringList()
{
    return NipaGames;
}

uint8_t INipaFile::GameStringToID(const string& String)
{
    for (int i = 0; NipaGames[i]; ++i)
        if (String == NipaGames[i])
            return i;
    return -1;
}

INipaFile::INipaFile(const string& Name, uint8_t GameID) : INpaFile(Name), GameID(GameID)
{
    ReadHeader();
}

INipaFile::~INipaFile()
{
}

void INipaFile::ReadHeader()
{
    ifstream File(Name, ios::binary);
    if (!File)
        return;

    File.read(NPAHeader.Magic, 7);
    assert(strncmp("NPA\x01\x00\x00\x00", NPAHeader.Magic, 7) == 0);

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
        NipaEntry* NPAEntry = new NipaEntry;
        File.read((char*)&NPAEntry->NameSize, 4);

        NPAEntry->Filename = new char[NPAEntry->NameSize + 1];
        File.read((char*)NPAEntry->Filename, NPAEntry->NameSize);

        for (int x = 0; x < NPAEntry->NameSize; ++x)
            NPAEntry->Filename[x] += Crypt(x, i);
        NPAEntry->Filename[NPAEntry->NameSize] = '\0';

        File.read((char*)&NPAEntry->Type, 1);
        File.read((char*)&NPAEntry->FileID, 4);
        File.read((char*)&NPAEntry->Offset, 4);
        File.read((char*)&NPAEntry->CompSize, 4);
        File.read((char*)&NPAEntry->Size, 4);

        string UtfPath = NpaFile::ToUtf8(NPAEntry->Filename);
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

int INipaFile::Crypt2(char* Filename, int32_t origsize)
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
        case TOTONO:
            key1 = 0x12345678;
            break;
        default:
            key1 = 0x87654321;
            break;
    }

    for (i = 0; Filename[i] != 0; ++i)
        key1 -= Filename[i];

    key = key1 * i;
    if (GameID != LAMENTO && GameID != LAMENTOTR)
    {
        key += key2;
        key *= origsize;
    }
    return key & 0xff;
}

char* INipaFile::ReadFile(NpaIterator iter)
{
    return ReadData(iter, 0, ((NipaEntry*)iter->second)->CompSize);
}

char* INipaFile::ReadData(NpaIterator iter, uint32_t LocalOffset, uint32_t Size, void *(*Alloc)(size_t))
{
    // Cannot offset into compressed file
    if (LocalOffset != 0 && NPAHeader.Compress == 1)
        return nullptr;

    ifstream File(Name, ios::binary);
    if (!File)
        return nullptr;

    NipaEntry* NPAEntry = (NipaEntry*)iter->second;
    uint8_t* buffer = new uint8_t[Size];

    File.seekg(NPAEntry->Offset + LocalOffset + NPAHeader.Start + 0x29);
    File.read((char*)buffer, Size);

    if (NPAHeader.Encrypt == 1)
    {
        int key = Crypt2(NPAEntry->Filename, NPAEntry->Size);
        int len = 0x1000;

        if (GameID != LAMENTO && GameID != LAMENTOTR)
            len += strlen(NPAEntry->Filename);

        for (int x = LocalOffset; x < Size + LocalOffset && x < len + LocalOffset; ++x)
        {
            if (GameID == TOTONO)
            {
                uint8_t r = buffer[x];
                char r2;
                r = NpaKeyTable[GameID][r];
                r = NpaKeyTable[GameID][r];
                r = NpaKeyTable[GameID][r];
                r = ~r;
                r2 = (char)r - key - x;
                buffer[x] = (uint8_t)r2;
            }
            else if (GameID == LAMENTO || GameID == LAMENTOTR)
                buffer[x] = NpaKeyTable[GameID][buffer[x]] - key;
            else
                buffer[x] = (NpaKeyTable[GameID][buffer[x]] - key) - x;
        }
    }

    if (NPAHeader.Compress == 1)
    {
        char* zbuffer = new char[NPAEntry->Size];
        uLongf sz = NPAEntry->Size;
        assert(uncompress((Bytef*)zbuffer, &sz, buffer, NPAEntry->CompSize) == Z_OK);
        delete[] buffer;
        return zbuffer;
    }
    return (char*)buffer;
}

bool INipaFile::IsDirectory(NpaIterator iter)
{
    return ((NipaEntry*)iter->second)->Type == 1;
}
