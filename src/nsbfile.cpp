#include "nsbfile.hpp"

#include <cassert>
#include <fstream>
#include <boost/assign/list_of.hpp>
#include <boost/unordered_map.hpp>

static const boost::unordered_map<uint16_t, const char*> MagicStrings = boost::assign::map_list_of
    (MAGIC_TEXT, "TEXT")
    (MAGIC_PARAM, "PARAM")
    (MAGIC_BEGIN, "BEGIN")
    (MAGIC_IF, "IF")
    (MAGIC_ENDIF, "ENDIF")
    (MAGIC_CALL, "CALL")

    (MAGIC_UNK1, "UNK1")
    (MAGIC_UNK2, "UNK2")
    (MAGIC_UNK3, "UNK3")
    (MAGIC_UNK4, "UNK4")
    (MAGIC_UNK5, "UNK5")
    (MAGIC_UNK6, "UNK6")
    (MAGIC_UNK7, "UNK7")
    (MAGIC_UNK8, "UNK8")
    (MAGIC_UNK9, "UNK9")

    (MAGIC_UNK10, "UNK10")
    (MAGIC_UNK11, "UNK11")
    (MAGIC_UNK12, "UNK12")
    (MAGIC_UNK13, "UNK13")
    (MAGIC_UNK14, "UNK14")
    (MAGIC_UNK15, "UNK15")
    (MAGIC_UNK16, "UNK16")
    (MAGIC_UNK17, "UNK17")
    (MAGIC_UNK18, "UNK18")
    (MAGIC_UNK19, "UNK19")
    (MAGIC_UNK20, "UNK20")
    (MAGIC_UNK21, "UNK21")
    (MAGIC_UNK22, "UNK22")
    (MAGIC_UNK23, "UNK23")
    (MAGIC_UNK24, "UNK24")
    (MAGIC_UNK25, "UNK25")
    (MAGIC_UNK26, "UNK26")
    (MAGIC_UNK27, "UNK27")
    (MAGIC_UNK28, "UNK28")
    (MAGIC_UNK29, "UNK29")
    (MAGIC_UNK30, "UNK30")
    (MAGIC_UNK31, "UNK31")
    (MAGIC_UNK32, "UNK32")
    (MAGIC_UNK33, "UNK33")
    (MAGIC_UNK34, "UNK34")
    (MAGIC_UNK35, "UNK35")
    (MAGIC_UNK36, "UNK36")
    (MAGIC_UNK37, "UNK37");

/* PUBLIC */

NsbFile::NsbFile(const std::string& Name, OpenMode Mode) :
SourceIter(0),
Name(Name)
{
    switch (Mode)
    {
        case NSB_COMPILED:
            ReadFromBinary();
            break;
        case NSB_PARSED:
            ReadFromSource();
            break;
        default:
            assert(false);
    }
}

const char* NsbFile::StringifyMagic(uint16_t Magic)
{
    return MagicStrings.at(Magic);
}

Line* NsbFile::GetNextLine()
{
    return SourceIter < Source.size() ? &Source[SourceIter++] : nullptr;
}

std::string NsbFile::GetName()
{
    return Name;
}

/* PRIVATE */

void NsbFile::ReadFromSource()
{
}

void NsbFile::ReadFromBinary()
{
    std::ifstream File(Name, std::ios::in | std::ios::binary);
    uint32_t Entry, Length;
    uint16_t NumParams;
    Line* CurrLine;

    // Find # of lines
    File.seekg(-2 * sizeof(uint32_t), File.end);
    File.read((char*)&Entry, sizeof(uint32_t));
    Source.resize(Entry);
    File.seekg(0, File.beg);

    // Read source code lines
    while (File.read((char*)&Entry, sizeof(uint32_t)))
    {
        CurrLine = &Source[Entry - 1];
        File.read((char*)&CurrLine->Magic, sizeof(uint16_t));
        File.read((char*)&NumParams, sizeof(uint16_t));
        CurrLine->Params.reserve(NumParams);

        // Read parameters
        for (uint16_t i = 0; i < NumParams; ++i)
        {
            File.read((char*)&Length, sizeof(uint32_t));
            char* String = new char[Length + 1];
            File.read(String, Length);
            String[Length] = 0;
            CurrLine->Params.push_back(String);
            delete[] String;
        }
    }
}
