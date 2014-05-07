#include "exefile.hpp"
#include "npafile.hpp"
#include <cstdint>
#include <cstring>

static const char PeSig[4] = { 'P', 'E', 0, 0 };

ExeFile::ExeFile(const std::string& Name) : Name(Name), ImageBase(0)
{
    std::ifstream File(Name, std::ios::binary);
    if (!File)
        return;

    // Signature
    File.seekg(0x3C);
    uint32_t PeOffset;
    File.read((char*)&PeOffset, 4);
    File.seekg(PeOffset);
    char Sig[4];
    File.read(Sig, 4);
    if (std::memcmp(Sig, PeSig, 4) != 0)
        return;

    // File header
    uint16_t NumSections;
    File.seekg(2, File.cur);
    File.read((char*)&NumSections, 2);
    Sections.resize(NumSections);
    File.seekg(16, File.cur);

    // Optional header
    File.seekg(28, File.cur);
    File.read((char*)&ImageBase, 4);
    File.seekg(192, File.cur);

    // Section header
    for (auto& i : Sections)
    {
        File.read((char*)&i.Name, 8);
        File.read((char*)&i.VirtualSize, 4);
        File.read((char*)&i.VirtualAddress, 4);
        File.read((char*)&i.SizeOfRawData, 4);
        File.read((char*)&i.PointerToRawData, 4);
        File.read((char*)&i.PointerToRelocations, 4);
        File.read((char*)&i.PointerToLineNumbers, 4);
        File.read((char*)&i.NumberOfRelocations, 2);
        File.read((char*)&i.NumberOfLinenumbers, 2);
        File.read((char*)&i.Characteristics, 4);
    }
}

uint32_t ExeFile::VirtualToPhysical(uint32_t Address)
{
    Address -= ImageBase;
    for (uint16_t i = 0; i < Sections.size(); ++i)
    {
        int32_t Diff = Address - Sections[i].VirtualAddress;
        if (Diff >= 0 && Diff < Sections[i].SizeOfRawData)
            return Diff + Sections[i].PointerToRawData;
    }
    return 0;
}

template <> std::string ExeFile::Read(uint32_t Address)
{
    std::string Ret;
    std::ifstream File(Name, std::ios::binary);
    uint32_t PhysicalAddress = VirtualToPhysical(Address);

    if (PhysicalAddress == 0)
        return Ret;

    File.seekg(PhysicalAddress);
    uint16_t Char;
    do
    {
        File.read((char*)&Char, 2);
        if (Char) Ret.append((char*)&Char, 2);
    } while (Char);
    return Ret.empty() ? Ret : NpaFile::ToUtf8(Ret);
}
