#include "npafile.hpp"

#include <cstring>

/* PUBLIC */

NpaFile::NpaFile(std::string Name) :
Header(nullptr),
Name(Name)
{
    Open(Name);
}

NpaFile::~NpaFile()
{
    Close();
}

void NpaFile::Open(std::string Name)
{
    File.open(Name, std::ios::in | std::ios::out | std::ios::binary);
    File.read((char*)&HeaderSize, sizeof(uint32_t));
    Header = new char[HeaderSize];
    ReadEncrypted(Header, sizeof(uint32_t), HeaderSize);
}

void NpaFile::Close()
{
    Flush();
    File.close();
    delete[] Header;
    Header = nullptr;
}

NpaIterator NpaFile::Begin()
{
    return NpaIterator(this, Header + sizeof(uint32_t));
}

NpaIterator NpaFile::End()
{
    return NpaIterator(this, nullptr);
}

void NpaFile::AppendFile(std::string Name, const char* buff, uint32_t size)
{
    ++EntryCount;

    // Header
    uint32_t i = NewHeader.size(), nsz = Name.size(), null = 0, ndsz = NewData.size();
    NewHeader.resize(NewHeader.size() + 4 * sizeof(uint32_t) + Name.size());
    std::memcpy(&NewHeader[i], &nsz, sizeof(uint32_t)); // Filename size
    std::memcpy(&NewHeader[i + 1 * sizeof(uint32_t)], &Name[0], nsz); // Filename
    std::memcpy(&NewHeader[i + 1 * sizeof(uint32_t) + nsz], &size, sizeof(uint32_t)); // File size
    std::memcpy(&NewHeader[i + 2 * sizeof(uint32_t) + nsz], &ndsz, sizeof(uint32_t)); // (local) File offset
    std::memcpy(&NewHeader[i + 3 * sizeof(uint32_t) + nsz], &null, sizeof(uint32_t)); // Unknown

    // Data
    NewData.resize(ndsz + size);
    std::memcpy(&NewData[ndsz], buff, size);
}

void NpaFile::AppendFile(std::string Name)
{
    std::ifstream In(Name, std::ios::in | std::ios::binary);
    In.seekg(0, In.end);
    uint32_t size = In.tellg();
    char* buff = new char[size];
    In.seekg(0, In.beg);
    AppendFile(Name, buff, size);
    delete[] buff;
}

/* PRIVATE */

static const uint8_t Key[] = { 0xBD, 0xAA, 0xBC, 0xB4, 0xAB, 0xB6, 0xBC, 0xB4 };

void NpaFile::ReadEncrypted(char* buff, uint32_t offset, uint32_t size)
{
    File.seekg(offset, File.beg);
    File.read(buff, size);
    XOR(buff, size, 0);
}

char* NpaFile::XOR(char* buff, uint32_t size, uint32_t keyoff)
{
    for (uint32_t i = 0; i < size; i++)
        buff[i] ^= Key[(i + keyoff) % sizeof(Key)];
    return buff;
}

void NpaFile::Flush()
{
    if (this->NewHeader.empty())
        return;

    std::FILE* pFile = std::fopen(std::string(Name + ".tmp").c_str(), "w");
    std::vector<char> NewHeader; // NOTE: Same name as this->NewHeader
    uint32_t offset, hpos = 4;
    bool old = true;

    // Take into consideration only files which were not removed
    for (NpaIterator i = Begin(); i != End(); ++i)
    {
        if (i.GetFileSize() == 0)
            continue;

        uint32_t pos = NewHeader.size();
        NewHeader.resize(NewHeader.size() + 4 * sizeof(uint32_t) + i.GetFileNameSize());
        std::memcpy(&NewHeader[pos], i.Pos, NewHeader.size() - pos);
    }

    fwrite(&EntryCount, sizeof(uint32_t), 1, pFile);

    // Write files to (sparse) achieve
    offset = NewHeader.size() + 4;
    for (NpaIterator i(this, (char*)&NewHeader[0]); i != End(); ++i)
    {
        // Switch to this->NewData
        if (i.GetOffset() == 0)
            old = false;

        std::fseek(pFile, offset, SEEK_SET);
        std::fwrite(XOR(old ? i.GetFileData() : &NewData[i.GetOffset()], i.GetFileSize(), 0),
                    sizeof(char), i.GetFileSize(), pFile);
        i.SetOffset(offset);
        offset += i.GetFileSize();
        std::fseek(pFile, hpos, SEEK_SET);
        std::fwrite(XOR(i.GetRawEntry(), i.GetRawEntrySize(), (hpos - 4) % sizeof(Key)),
                    sizeof(char), i.GetRawEntrySize(), pFile);
        hpos += i.GetRawEntrySize();
    }

    std::rename(std::string(Name + ".tmp").c_str(), Name.c_str());
}
