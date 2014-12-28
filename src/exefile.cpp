#include "exefile.hpp"
#include "npafile.hpp"
#include <cstdint>
#include <cstring>

static const char PeSig[4] = { 'P', 'E', 0, 0 };

ExeFile::ExeFile(const string& Name, uint8_t CharWidth) :
Name(Name),
ImageBase(0),
CharWidth(CharWidth)
{
    ifstream File(Name, ios::binary);
    if (!File)
        return;

    // Signature
    File.seekg(0x3C);
    uint32_t PeOffset;
    File.read((char*)&PeOffset, 4);
    File.seekg(PeOffset);
    char Sig[4];
    File.read(Sig, 4);
    if (memcmp(Sig, PeSig, 4) != 0)
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
        File.seekg(12, File.cur);
        File.read((char*)&i.VirtualAddress, 4);
        File.read((char*)&i.SizeOfRawData, 4);
        File.read((char*)&i.PointerToRawData, 4);
        File.seekg(16, File.cur);
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

template <> string ExeFile::Read(uint32_t Address)
{
    string Ret;
    ifstream File(Name, ios::binary);
    uint32_t PhysicalAddress = VirtualToPhysical(Address);

    if (PhysicalAddress == 0)
        return Ret;

    File.seekg(PhysicalAddress);
    uint16_t Char;
    do
    {
        File.read((char*)&Char, CharWidth);
        if (Char) Ret.append((char*)&Char, CharWidth);
    } while (Char);
    return Ret.empty() ? Ret : NpaFile::ToUtf8(Ret);
}
