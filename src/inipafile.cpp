#include "inipafile.hpp"
#include "fscommon.hpp"
#include "buffer.hpp"
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
    uint32_t Size;
    char* pData = fs::ReadFile(Name, Size);
    if (!pData)
        return;

    Npa::Buffer File(pData, Size);
    File.Read(NPAHeader.Magic, 7);
    assert(strncmp("NPA\x01\x00\x00\x00", NPAHeader.Magic, 7) == 0);

    NPAHeader.Key1 = File.Read<int32_t>();
    NPAHeader.Key2 = File.Read<int32_t>();
    NPAHeader.Compress = File.Read<uint8_t>();
    NPAHeader.Encrypt = File.Read<uint8_t>();
    NPAHeader.TotalCount = File.Read<uint32_t>();
    NPAHeader.FolderCount = File.Read<uint32_t>();
    NPAHeader.FileCount = File.Read<uint32_t>();
    NPAHeader.Unk = File.Read<uint64_t>();
    NPAHeader.Start = File.Read<uint32_t>();

    for (uint32_t i = 0; i < NPAHeader.TotalCount; ++i)
    {
        NipaEntry* NPAEntry = new NipaEntry;
        NPAEntry->Filename = File.ReadStr32();
        for (int x = 0; x < NPAEntry->Filename.size(); ++x)
            NPAEntry->Filename[x] += FilenameCrypt(x, i);

        NPAEntry->Type = File.Read<uint8_t>();
        NPAEntry->FileID = File.Read<uint32_t>();
        NPAEntry->Offset = File.Read<uint32_t>();
        NPAEntry->CompSize = File.Read<uint32_t>();
        NPAEntry->Size = File.Read<uint32_t>();

        string UtfPath = NpaFile::ToUtf8(NPAEntry->Filename);
        boost::replace_all(UtfPath, "\\", "/");
        Registry[UtfPath] = NPAEntry;
    }
}

char INipaFile::FilenameCrypt(int32_t CharIndex, int32_t FileIndex)
{
    int32_t Key = 0xFC * CharIndex;
    int32_t Temp = 0;

    if (NPAHeader.Encrypt && (GameID == LAMENTO || GameID == LAMENTOTR))
        Temp = NPAHeader.Key1 + NPAHeader.Key2;
    else
        Temp = NPAHeader.Key1 * NPAHeader.Key2;

    Key -= Temp >> 0x18;
    Key -= Temp >> 0x10;
    Key -= Temp >> 0x08;
    Key -= Temp & 0xff;
    Key -= FileIndex >> 0x18;
    Key -= FileIndex >> 0x10;
    Key -= FileIndex >> 0x08;
    Key -= FileIndex;
    return Key & 0xff;
}

int INipaFile::Crypt2(const char* Filename, int32_t origsize)
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
        int key = Crypt2(NPAEntry->Filename.c_str(), NPAEntry->Size);
        int len = 0x1000;

        if (GameID != LAMENTO && GameID != LAMENTOTR)
            len += NPAEntry->Filename.size();

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
