#ifndef EXE_FILE_HPP
#define EXE_FILE_HPP

#include <string>
#include <vector>
#include <fstream>
using namespace std;

class ExeFile
{
    struct SectionHeader
    {
        uint32_t VirtualAddress;
        uint32_t SizeOfRawData;
        uint32_t PointerToRawData;
    };
public:
    ExeFile(const string& Name, uint8_t CharWidth);
    template <class T> T Read(uint32_t Address);

private:
    uint32_t VirtualToPhysical(uint32_t Address);

    uint8_t CharWidth;
    uint32_t ImageBase;
    const string Name;
    vector<SectionHeader> Sections;
};

template <class T> T ExeFile::Read(uint32_t Address)
{
    T Ret;
    ifstream File(Name, ios::binary);
    uint32_t PhysicalAddress = VirtualToPhysical(Address);

    if (PhysicalAddress == 0)
        return T();

    File.seekg(PhysicalAddress);
    File.read((char*)&Ret, sizeof(T));
    return Ret;
}

template <> string ExeFile::Read(uint32_t Address);

#endif
