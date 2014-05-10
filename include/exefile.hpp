#ifndef EXE_FILE_HPP
#define EXE_FILE_HPP

#include <string>
#include <vector>
#include <fstream>

class ExeFile
{
    struct SectionHeader
    {
        char Name[8];
        uint32_t VirtualSize;
        uint32_t VirtualAddress;
        uint32_t SizeOfRawData;
        uint32_t PointerToRawData;
        uint32_t PointerToRelocations;
        uint32_t PointerToLineNumbers;
        uint16_t NumberOfRelocations;
        uint16_t NumberOfLinenumbers;
        uint32_t Characteristics;
    };
public:
    ExeFile(const std::string& Name, uint8_t CharWidth);
    template <class T> T Read(uint32_t Address);

private:
    uint32_t VirtualToPhysical(uint32_t Address);

    uint8_t CharWidth;
    uint32_t ImageBase;
    const std::string Name;
    std::vector<SectionHeader> Sections;
};

template <class T> T ExeFile::Read(uint32_t Address)
{
    T Ret;
    std::ifstream File(Name, std::ios::binary);
    uint32_t PhysicalAddress = VirtualToPhysical(Address);

    if (PhysicalAddress == 0)
        return T();

    File.seekg(PhysicalAddress);
    File.read((char*)&Ret, sizeof(T));
    return Ret;
}

template <> std::string ExeFile::Read(uint32_t Address);

#endif
