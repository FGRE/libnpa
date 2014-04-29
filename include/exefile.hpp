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
    ExeFile(const std::string& Name);
    template <class T> void Read(T& Dest, uint32_t Address);

private:
    uint32_t VirtualToPhysical(uint32_t Address);
    uint32_t ImageBase;
    const std::string Name;
    std::vector<SectionHeader> Sections;
};

template <class T> void ExeFile::Read(T& Dest, uint32_t Address)
{
    std::ifstream File(Name, std::ios::binary);
    File.seekg(VirtualToPhysical(Address));
    File.read((char*)&Dest, sizeof(T));
}

#endif
