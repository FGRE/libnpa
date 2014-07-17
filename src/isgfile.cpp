#include "isgfile.hpp"
#include <fstream>

void* DefaultAlloc(size_t Size)
{
    return new char[Size];
}

ISGFile::ISGFile(const std::string& Filename) : INpaFile(Filename)
{
    ReadHeader();
}

ISGFile::~ISGFile()
{
}

void ISGFile::ReadHeader()
{
    std::ifstream File(Name, std::ios::binary);
    assert(File);

    uint32_t HeaderSize, EntryCount;
    File.read((char*)&HeaderSize, 4);
    char* pHeader = new char[HeaderSize];
    char* pHeaderBeg = pHeader;
    File.read(pHeader, HeaderSize);
    Decrypt(pHeader, HeaderSize);
    EntryCount = *(uint32_t*)pHeader;
    pHeader += 4;

    for (uint32_t i = 0; i < EntryCount; ++i)
    {
        uint32_t FilenameSize = *(uint32_t*)pHeader;
        pHeader += 4;
        std::string Filename(pHeader, FilenameSize);
        pHeader += FilenameSize;
        SGEntry* pEntry = new SGEntry;
        pEntry->Size = *(uint32_t*)pHeader;
        pEntry->Offset = *(uint32_t*)(pHeader + sizeof(uint32_t));
        pEntry->Unk = *(uint32_t*)(pHeader + 2 * sizeof(uint32_t));
        Registry[ToUtf8(Filename)] = pEntry;
        pHeader += 3 * sizeof(uint32_t);
    }

    delete[] pHeaderBeg;
}

char* ISGFile::ReadData(uint32_t GlobalOffset, uint32_t LocalOffset, uint32_t Size, void *(*Alloc)(size_t))
{
    std::ifstream File(Name, std::ios::binary);
    if (!File)
        return nullptr;

    char* pData = (char*)Alloc(Size);
    File.seekg(GlobalOffset + LocalOffset, File.beg);
    File.read(pData, Size);
    Decrypt(pData, Size, LocalOffset);
    return pData;
}

bool ISGFile::IsDirectory(NpaIterator iter)
{
    return false;
}
