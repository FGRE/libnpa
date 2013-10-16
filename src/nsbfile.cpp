#include "nsbfile.hpp"

#include <cassert>
#include <fstream>
#include <boost/assign/list_of.hpp>
#include <boost/unordered_map.hpp>

static const boost::unordered_map<uint16_t, const char*> MagicStrings = boost::assign::map_list_of
    (MAGIC_TEXT, "TEXT")
    (MAGIC_PARAM, "SetParam")
    (MAGIC_UNK0, "UNK0")
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
    (MAGIC_CONCAT, "Concat")

    (MAGIC_SET, "Set")
    (MAGIC_GET, "Get")
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
    (MAGIC_MUSIC_STOP, "StopMusic")
    (MAGIC_UNK25, "UNK25")
    (MAGIC_UNK26, "UNK26")
    (MAGIC_UNK27, "UNK27")
    (MAGIC_UNK28, "UNK28")
    (MAGIC_UNK29, "UNK29")
    (MAGIC_LOAD_MOVIE, "LoadMovie")
    (MAGIC_UNK31, "UNK31")
    (MAGIC_UNK32, "UNK32")
    (MAGIC_UNK33, "UNK33")
    (MAGIC_UNK34, "UNK34")
    (MAGIC_UNK35, "UNK35")
    (MAGIC_BEGIN, "BEGIN")
    (MAGIC_END,   "END")
    (MAGIC_UNK38, "UNK38")
    (MAGIC_UNK39, "UNK39")
    (MAGIC_UNK40, "UNK40")
    (MAGIC_UNK41, "UNK41")
    (MAGIC_UNK42, "UNK42")
    (MAGIC_UNK43, "UNK43")
    (MAGIC_UNK44, "UNK44")
    (MAGIC_UNK45, "UNK45")
    (MAGIC_UNK46, "UNK46")
    (MAGIC_UNK47, "UNK47")
    (MAGIC_UNK48, "UNK48")
    (MAGIC_UNK49, "UNK49")
    (MAGIC_UNK50, "UNK50")
    (MAGIC_UNK51, "UNK51")
    (MAGIC_UNK52, "UNK52")
    (MAGIC_UNK53, "UNK53")
    (MAGIC_UNK54, "UNK54")
    (MAGIC_UNK55, "UNK55")
    (MAGIC_UNK56, "UNK56")
    (MAGIC_UNK57, "UNK57")
    (MAGIC_UNK58, "UNK58")
    (MAGIC_UNK59, "UNK59")
    (MAGIC_UNK60, "UNK60")
    (MAGIC_UNK61, "UNK61")
    (MAGIC_UNK62, "UNK62")
    (MAGIC_UNK63, "UNK63")
    (MAGIC_UNK64, "UNK64")
    (MAGIC_UNK65, "UNK65")
    (MAGIC_UNK66, "UNK66")
    (MAGIC_UNK67, "UNK67")
    (MAGIC_UNK68, "UNK68")
    (MAGIC_UNK69, "UNK69")
    (MAGIC_UNK70, "UNK70")
    (MAGIC_UNK71, "UNK71")
    (MAGIC_UNK72, "UNK72")
    (MAGIC_UNK73, "UNK73")
    (MAGIC_UNK74, "UNK74")
    (MAGIC_UNK75, "UNK75")
    (MAGIC_UNK76, "UNK76")
    (MAGIC_UNK77, "UNK77")
    (MAGIC_UNK78, "UNK78")
    (MAGIC_UNK79, "UNK79")
    (MAGIC_UNK80, "UNK80")
    (MAGIC_UNK81, "UNK81")
    (MAGIC_UNK82, "UNK82")
    (MAGIC_UNK83, "UNK83")
    (MAGIC_UNK84, "UNK84")
    (MAGIC_UNK85, "UNK85")
    (MAGIC_CALL_SCRIPT, "CallScript")
    (MAGIC_UNK87, "UNK87")
    (MAGIC_UNK88, "UNK88")
    (MAGIC_UNK89, "UNK89")
    (MAGIC_UNK90, "UNK90")
    (MAGIC_UNK91, "UNK91")
    (MAGIC_UNK92, "UNK92")
    (MAGIC_UNK93, "UNK93")
    (MAGIC_UNK94, "UNK94")
    (MAGIC_UNK95, "UNK95")
    (MAGIC_UNK96, "UNK96")
    (MAGIC_UNK97, "UNK97")
    (MAGIC_UNK98, "UNK98")
    (MAGIC_UNK99, "UNK99")
    (MAGIC_UNK100, "UNK100")
    (MAGIC_UNK101, "UNK101");

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

bool NsbFile::IsValidMagic(uint16_t Magic)
{
    return MagicStrings.find(Magic) != MagicStrings.end();
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
