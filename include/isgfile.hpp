#ifndef I_SG_FILE_HPP
#define I_SG_FILE_HPP

#include "inpafile.hpp"
#include <map>

class ISGFile : public INpaFile
{
public:
    ISGFile(const std::string& Filename);
    ~ISGFile();

    struct SGEntry : INpaFile::Entry
    {
        uint32_t Unk;
    };

    char* ReadFile(const std::string& Filename, uint32_t& Size);
    char* ReadFile(NpaIterator iter, uint32_t Offset, uint32_t Size, void *(*Alloc)(size_t) = DefaultAlloc);

private:
    char* ReadData(uint32_t GlobalOffset, uint32_t LocalOffset, uint32_t Size, void *(*Alloc)(size_t));
    void ReadHeader();
};

#endif
