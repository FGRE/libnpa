#ifndef I_NIPA_FILE
#define I_NIPA_FILE

#include "inpafile.hpp"

class IMpkFile : public INpaFile
{
    struct
    {
        char Magic[6]; // M P K \0 \0 \0
        uint16_t Version; // \x02 \0
        uint32_t FileCount;
        char Pad[0x34];
    } MPKHeader;
public:
    struct MpkEntry : INpaFile::Entry
    {
        uint32_t Unk;
        string Filename;
        uint32_t FileID;
    };

    IMpkFile(const string& Name);
    ~IMpkFile();

    char* ReadFile(NpaIterator iter);
    virtual char* ReadData(NpaIterator iter, uint32_t LocalOffset, uint32_t Size, void *(*Alloc)(size_t) = DefaultAlloc);
    bool IsDirectory(NpaIterator iter);

protected:
    void ReadHeader();
};

#endif
