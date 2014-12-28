#include "isgfile.hpp"
#include "buffer.hpp"
#include <fstream>

void* DefaultAlloc(size_t Size)
{
    return new char[Size];
}

ISGFile::ISGFile(const string& Filename) : INpaFile(Filename)
{
    ReadHeader();
}

ISGFile::~ISGFile()
{
}

void ISGFile::ReadHeader()
{
    ifstream File(Name, ios::binary);
    if (!File)
        return;

    uint32_t HeaderSize;
    File.read((char*)&HeaderSize, 4);
    Npa::Buffer SGBuffer(new char[HeaderSize], HeaderSize);
    File.read(SGBuffer.GetData(), HeaderSize);
    Decrypt(SGBuffer.GetData(), HeaderSize);

    uint32_t EntryCount = SGBuffer.Read<uint32_t>();
    for (uint32_t i = 0; i < EntryCount; ++i)
    {
        string Filename = SGBuffer.ReadStr32();
        SGEntry* pEntry = new SGEntry;
        pEntry->Size = SGBuffer.Read<uint32_t>();
        pEntry->Offset = SGBuffer.Read<uint32_t>();
        pEntry->Unk = SGBuffer.Read<uint32_t>();
        Registry[ToUtf8(Filename)] = pEntry;
    }
}

char* ISGFile::ReadFile(NpaIterator iter)
{
    return ReadData(iter, 0, GetFileSize(iter));
}

char* ISGFile::ReadData(NpaIterator iter, uint32_t LocalOffset, uint32_t Size, void *(*Alloc)(size_t))
{
    ifstream File(Name, ios::binary);
    if (!File)
        return nullptr;

    char* pData = (char*)Alloc(Size);
    File.seekg(iter->second->Offset + LocalOffset, File.beg);
    File.read(pData, Size);
    Decrypt(pData, Size, LocalOffset);
    return pData;
}

bool ISGFile::IsDirectory(NpaIterator iter)
{
    return false;
}
