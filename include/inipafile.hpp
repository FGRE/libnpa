#ifndef I_NIPA_FILE
#define I_NIPA_FILE

#include "inpafile.hpp"

class INipaFile : public INpaFile
{
    struct
    {
        char Magic[7];
        int32_t Key1;
        int32_t Key2;
        uint8_t Encrypt;
        uint8_t Compress;
        uint32_t FileCount;
        uint32_t FolderCount;
        uint32_t TotalCount;
        uint64_t Unk;
        uint32_t Start;
    } NPAHeader;
public:
    enum
    {
        CHAOSHEAD = 0,
        CHAOSHEADTR1,
        CHAOSHEADTR2,
        MURAMASATR,
        MURAMASA,
        SUMAGA,
        DJANGO,
        DJANGOTR,
        LAMENTO,
        LAMENTOTR,
        SWEETPOOL,
        SUMAGASP,
        DEMONBANE,
        MURAMASAAD,
        AXANAEL,
        KIKOKUGAI,
        SONICOMITR2,
        SUMAGA3P,
        SONICOMI,
        LOSTX,
        LOSTXTRAILER,
        DRAMATICALMURDER
    };

    struct NipaEntry : INpaFile::Entry
    {
        uint32_t NameSize;
        char* Filename;
        uint8_t Type;
        uint32_t FileID;
        uint32_t CompSize;
    };

    INipaFile(const std::string& Name, uint8_t GameID);
    ~INipaFile();

    char* ReadFile(NpaIterator iter, uint32_t Offset, uint32_t Size, void *(*Alloc)(size_t));
    char* ReadFile(const std::string& Filename, uint32_t& Size);
    char* ReadFile(NpaIterator iter);

protected:
    char* ReadData(uint32_t GlobalOffset, uint32_t LocalOffset, uint32_t Size, void *(*Alloc)(size_t)) {}

    void ReadHeader();
    int Crypt(int32_t curnum, int32_t curfile);
    int Crypt2(char* name, int32_t origsize);

    uint8_t GameID;
};

#endif
