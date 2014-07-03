#ifndef I_SG_FILE_HPP
#define I_SG_FILE_HPP

#include "inpafile.hpp"
#include <map>

struct SGEntry
{
    uint32_t Size;
    uint32_t Offset;
    uint32_t Unk;
};

class ISGFile : public INpaFile<SGEntry>
{
public:
    ISGFile(const std::string& Filename);
    ~ISGFile();

    char* ReadFile(const std::string& Filename, uint32_t& Size);
    char* ReadFile(NpaIterator iter, uint32_t Offset, uint32_t Size, void *(*Alloc)(size_t) = DefaultAlloc);
    uint32_t GetFileSize(NpaIterator iter) { return iter->second.Size; }

private:
    void ReadHeader();
    char* ReadData(uint32_t GlobalOffset, uint32_t LocalOffset, uint32_t Size, void *(*Alloc)(size_t));
};

#endif
