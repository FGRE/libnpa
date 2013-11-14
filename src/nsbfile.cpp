#include "nsbfile.hpp"
#include "nsbmagic.hpp"

#include <cassert>
#include <fstream>
#include <iconv.h>
#include <boost/assign/list_of.hpp>
#include <boost/bimap.hpp>

typedef boost::bimap<uint16_t, std::string> LookupTable;

static const LookupTable MagicStrings = boost::assign::list_of<LookupTable::value_type>
    (MAGIC_TEXT, "ParseText")
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
    (MAGIC_START_ANIMATION, "StartAnimation")
    (MAGIC_SET_DISPLAY_STATE, "SetDisplayState")
    (MAGIC_DESTROY, "Destroy")
    (MAGIC_DISPLAY, "Display")
    (MAGIC_UNK20, "UNK20")
    (MAGIC_SLEEP_MS, "SleepMs")
    (MAGIC_SET_NULL_PARAM, "SetNullParam")
    (MAGIC_UNK23, "UNK23")
    (MAGIC_SET_AUDIO_STATE, "SetAudioState")
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
    (MAGIC_LOAD_TEXTURE, "LoadTexture")
    (MAGIC_UNK42, "UNK42")
    (MAGIC_UNK43, "UNK43")
    (MAGIC_UNK44, "UNK44")
    (MAGIC_UNK45, "UNK45")
    (MAGIC_UNK46, "UNK46")
    (MAGIC_GET_MOVIE_TIME, "GetMovieTime")
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
    (MAGIC_SET_FONT_ATTRIBUTES, "SetFontAttributes")
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
    (MAGIC_UNK101, "UNK101")
    (MAGIC_CALLBACK, "Callback")
    (MAGIC_UNK103, "UNK103")
    (MAGIC_UNK104, "UNK104")
    (MAGIC_UNK105, "UNK105")
    (MAGIC_UNK106, "UNK106")
    (MAGIC_UNK107, "UNK107")
    (MAGIC_UNK108, "UNK108")
    (MAGIC_UNK109, "UNK109")
    (MAGIC_UNK110, "UNK110")
    (MAGIC_UNK111, "UNK111")
    (MAGIC_UNK112, "UNK112")
    (MAGIC_UNK113, "UNK113")
    (MAGIC_UNK114, "UNK114")
    (MAGIC_UNK115, "UNK115")
    (MAGIC_UNK116, "UNK116")
    (MAGIC_UNK117, "UNK117")
    (MAGIC_UNK118, "UNK118")
    (MAGIC_UNK119, "UNK119")
    (MAGIC_UNK120, "UNK120")
    (MAGIC_UNK121, "UNK121")
    (MAGIC_UNK122, "UNK122")
    (MAGIC_UNK123, "UNK123")
    (MAGIC_UNK124, "UNK124")
    (MAGIC_UNK125, "UNK125")
    (MAGIC_UNK126, "UNK126")
    (MAGIC_UNK127, "UNK127")
    (MAGIC_UNK128, "UNK128")
    (MAGIC_UNK129, "UNK129")
    (MAGIC_UNK130, "UNK130")
    (MAGIC_UNK131, "UNK131")
    (MAGIC_UNK132, "UNK132")
    (MAGIC_UNK133, "UNK133")
    (MAGIC_UNK134, "UNK134")
    (MAGIC_UNK135, "UNK135")
    (MAGIC_UNK136, "UNK136")
    (MAGIC_UNK137, "UNK137")
    (MAGIC_UNK138, "UNK138")
    (MAGIC_UNK139, "UNK139")
    (MAGIC_UNK140, "UNK140")
    (MAGIC_UNK141, "UNK141")
    (MAGIC_UNK142, "UNK142")
    (MAGIC_UNK143, "UNK143")
    (MAGIC_UNK144, "UNK144")
    (MAGIC_UNK145, "UNK145")
    (MAGIC_UNK146, "UNK146")
    (MAGIC_UNK147, "UNK147")
    (MAGIC_UNK148, "UNK148")
    (MAGIC_UNK149, "UNK149")
    (MAGIC_UNK150, "UNK150")
    (MAGIC_UNK151, "UNK151")
    (MAGIC_UNK152, "UNK152")
    (MAGIC_UNK153, "UNK153")
    (MAGIC_UNK154, "UNK154")
    (MAGIC_LOAD_AUDIO, "LoadAudio")
    (MAGIC_SET_AUDIO_RANGE, "SetAudioRange")
    (MAGIC_UNK157, "UNK157")
    (MAGIC_UNK158, "UNK158")
    (MAGIC_SET_AUDIO_LOOP, "SetAudioLoop")
    (MAGIC_UNK160, "UNK160")
    (MAGIC_UNK161, "UNK161")
    (MAGIC_UNK162, "UNK162")
    (MAGIC_SET_TEXTBOX_ATTRIBUTES, "SetTextboxAttributes")
    (MAGIC_UNK164, "UNK164")
    (MAGIC_UNK165, "UNK165")
    (MAGIC_UNK166, "UNK166")
    (MAGIC_UNK167, "UNK167")
    (MAGIC_UNK168, "UNK168")
    (MAGIC_UNK169, "UNK169")
    (MAGIC_UNK170, "UNK170")
    (MAGIC_UNK171, "UNK171")
    (MAGIC_UNK172, "UNK172");

/* PUBLIC */

NsbFile::NsbFile(const std::string& Name, char* Data, uint32_t Size)
{
    Open(Name, Data, Size);
}

bool NsbFile::IsValidMagic(uint16_t Magic)
{
    return MagicStrings.left.find(Magic) != MagicStrings.left.end();
}

const char* NsbFile::StringifyMagic(uint16_t Magic)
{
    auto iter = MagicStrings.left.find(Magic);
    if (iter != MagicStrings.left.end())
        return iter->second.c_str();
    return nullptr;
}

uint16_t NsbFile::MagicifyString(const char* String)
{
    auto iter = MagicStrings.right.find(String);
    if (iter != MagicStrings.right.end())
        return iter->second;
    return 0;
}

uint32_t NsbFile::GetFunctionLine(const char* Name) const
{
    auto iter = Functions.find(Name);
    if (iter != Functions.end())
        return iter->second;
    return 0;
}

/* PRIVATE */

#define MEGABYTE 1024 * 1024

void NsbFile::Read(std::istream* pStream)
{
    uint32_t Entry, Length;
    uint16_t NumParams;
    Line* CurrLine;
    iconv_t conv = iconv_open("UTF-8", "SHIFT-JIS");
    char* ConvBuff = new char[MEGABYTE];

    // No text, no game
    assert(conv != (iconv_t)-1);

    // Read source code lines
    while (pStream->read((char*)&Entry, sizeof(uint32_t)))
    {
        Source.resize(Source.size() + 1);
        CurrLine = &Source[Entry - 1];
        pStream->read((char*)&CurrLine->Magic, sizeof(uint16_t));
        pStream->read((char*)&NumParams, sizeof(uint16_t));
        CurrLine->Params.reserve(NumParams);

        // Read parameters
        for (uint16_t i = 0; i < NumParams; ++i)
        {
            pStream->read((char*)&Length, sizeof(uint32_t));
            char* String = new char[Length];
            pStream->read(String, Length);
            char *inbuff = String, *outbuff = ConvBuff;
            size_t BuffSize = MEGABYTE, inleft = Length;
            assert(iconv(conv, &inbuff, &inleft, &outbuff, &BuffSize) != (size_t)-1);
            CurrLine->Params.push_back(std::string(ConvBuff, MEGABYTE - BuffSize));
            delete[] String;
        }

        // Map function
        if (CurrLine->Magic == uint16_t(MAGIC_BEGIN))
            Functions[CurrLine->Params[0].c_str() + strlen("function.")] = Entry;
    }

    delete[] ConvBuff;
    iconv_close(conv);
}
