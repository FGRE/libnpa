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

    char* ReadFile(NpaIterator iter);
    virtual char* ReadData(NpaIterator iter, uint32_t LocalOffset, uint32_t Size, void *(*Alloc)(size_t) = DefaultAlloc);
    bool IsDirectory(NpaIterator iter);

private:
    void ReadHeader();
};

#endif
