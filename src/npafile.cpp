#include "npafile.hpp"

/* PUBLIC */

NpaFile::NpaFile(std::string Name) :
Header(nullptr)
{
    Open(Name);
}

NpaFile::~NpaFile()
{
    Close();
}

void NpaFile::Open(std::string Name)
{
    File.open(Name, std::ios::in | std::ios::binary);
    File.read((char*)&HeaderSize, sizeof(uint32_t));
    Header = new char[HeaderSize];
    ReadEncrypted(Header, sizeof(uint32_t), HeaderSize);
}

void NpaFile::Close()
{
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

/* PRIVATE */

void NpaFile::ReadEncrypted(char* buff, uint32_t offset, uint32_t size)
{
    static const uint8_t Key[] = { 0xBD, 0xAA, 0xBC, 0xB4, 0xAB, 0xB6, 0xBC, 0xB4 };

    File.seekg(offset, File.beg);
    File.read(buff, size);
    for (uint32_t i = 0; i < size; i++)
        buff[i] ^= Key[i % sizeof(Key)];
}
