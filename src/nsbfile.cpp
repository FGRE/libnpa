#include "nsbfile.hpp"
#include "nsbmagic.hpp"

#include <cassert>
#include <fstream>
#include <boost/assign/list_of.hpp>
#include <boost/bimap.hpp>
#include <boost/locale.hpp>
using namespace boost::locale;
using namespace boost::locale::conv;

typedef boost::bimap<uint16_t, std::string> LookupTable;

static const LookupTable MagicStrings = boost::assign::list_of<LookupTable::value_type>
    (MAGIC_FWN_UNK, "FUWANOVEL")

    (MAGIC_PARSE_TEXT, "ParseText")
    (MAGIC_SET_PARAM, "SetParam")
    (MAGIC_CHAPTER_BEGIN, "ChapterBegin")
    (MAGIC_IF, "If")
    (MAGIC_LABEL, "Label")
    (MAGIC_CALL, "Call")
    (MAGIC_UNK1, "UNK1")
    (MAGIC_UNK2, "UNK2")
    (MAGIC_SCOPE_BEGIN, "ScopeBegin")
    (MAGIC_SCOPE_END, "ScopeEnd")
    (MAGIC_GET_SCRIPT_NAME, "GetScriptName")
    (MAGIC_CLEAR_PARAMS, "ClearParams")
    (MAGIC_UNK7, "UNK7")
    (MAGIC_LOGICAL_NOT_EQUAL, "LogicalNotEqual")
    (MAGIC_ADD, "Add")
    (MAGIC_SET, "Set")
    (MAGIC_GET, "Get")
    (MAGIC_CHAPTER_END, "ChapterEnd")
    (MAGIC_SCENE_BEGIN, "SceneBegin")
    (MAGIC_CALL_SCENE, "CallScene")
    (MAGIC_SCENE_END, "SceneEnd")
    (MAGIC_MOVE, "Move")
    (MAGIC_REQUEST, "Request")
    (MAGIC_DELETE, "Delete")
    (MAGIC_FADE, "Fade")
    (MAGIC_UNK20, "UNK20")
    (MAGIC_WAIT, "Wait")
    (MAGIC_PLACEHOLDER_PARAM, "PlaceholderParam")
    (MAGIC_NEGATIVE, "Negative")
    (MAGIC_SET_VOLUME, "SetVolume")
    (MAGIC_SHAKE, "Shake")
    (MAGIC_ARRAY_READ, "ArrayRead")
    (MAGIC_UNK27, "UNK27")
    (MAGIC_CREATE_WINDOW, "CreateWindow")
    (MAGIC_SET_ALIAS, "SetAlias")
    (MAGIC_CREATE_MOVIE, "CreateMovie")
    (MAGIC_CENTER, "Center")
    (MAGIC_APPLY_BLUR, "ApplyBlur")
    (MAGIC_CREATE_RENDER_TEXTURE, "CreateRenderTexture")
    (MAGIC_DRAW_TO_TEXTURE, "DrawToTexture")
    (MAGIC_LOGICAL_EQUAL, "LogicalEqual")
    (MAGIC_FUNCTION_BEGIN, "FunctionBegin")
    (MAGIC_FUNCTION_END, "FunctionEnd")
    (MAGIC_UNK38, "UNK38")
    (MAGIC_UNK39, "UNK39")
    (MAGIC_UNK40, "UNK40")
    (MAGIC_CREATE_TEXTURE, "CreateTexture")
    (MAGIC_TEXTURE_WIDTH, "TextureWidth")
    (MAGIC_DIVIDE, "Divide")
    (MAGIC_MULTIPLY, "Multiply")
    (MAGIC_LOGICAL_NOT, "LogicalNot")
    (MAGIC_WHILE, "While")
    (MAGIC_GET_MOVIE_TIME, "GetMovieTime")
    (MAGIC_UNK48, "UNK48")
    (MAGIC_UNK49, "UNK49")
    (MAGIC_UNK50, "UNK50")
    (MAGIC_UNK51, "UNK51")
    (MAGIC_UNK52, "UNK52")
    (MAGIC_UNK53, "UNK53")
    (MAGIC_INCREMENT, "Increment")
    (MAGIC_LOGICAL_GREATER, "LogicalGreater")
    (MAGIC_DRAW_TRANSITION, "DrawTransition")
    (MAGIC_UNK57, "UNK57")
    (MAGIC_SUBSTRACT, "Substract")
    (MAGIC_UNK59, "UNK59")
    (MAGIC_UNK60, "UNK60")
    (MAGIC_UNK61, "UNK61")
    (MAGIC_UNK62, "UNK62")
    (MAGIC_UNK63, "UNK63")
    (MAGIC_LOGICAL_LESS, "LogicalLess")
    (MAGIC_LOOP_JUMP, "LoopJump")
    (MAGIC_UNK66, "UNK66")
    (MAGIC_UNK67, "UNK67")
    (MAGIC_UNK68, "UNK68")
    (MAGIC_CREATE_ARRAY, "CreateArray")
    (MAGIC_BIND_IDENTIFIER, "BindIdentifier")
    (MAGIC_UNK71, "UNK71")
    (MAGIC_TEXTURE_HEIGHT, "TextureHeight")
    (MAGIC_ARRAY_SIZE, "ArraySize")
    (MAGIC_SET_FONT_ATTRIBUTES, "SetFontAttributes")
    (MAGIC_UNK75, "UNK75")
    (MAGIC_CREATE_COLOR, "CreateColor")
    (MAGIC_UNK77, "UNK77")
    (MAGIC_CREATE_SCROLLBAR, "CreateScrollbar")
    (MAGIC_UNK79, "UNK79")
    (MAGIC_UNK80, "UNK80")
    (MAGIC_UNK81, "UNK81")
    (MAGIC_UNK82, "UNK82")
    (MAGIC_FORMAT, "Format")
    (MAGIC_UNK84, "UNK84")
    (MAGIC_UNK85, "UNK85")
    (MAGIC_CALL_CHAPTER, "CallChapter")
    (MAGIC_UNK87, "UNK87")
    (MAGIC_UNK88, "UNK88")
    (MAGIC_UNK89, "UNK89")
    (MAGIC_UNK90, "UNK90")
    (MAGIC_UNK91, "UNK91")
    (MAGIC_UNK92, "UNK92")
    (MAGIC_UNK93, "UNK93")
    (MAGIC_UNK94, "UNK94")
    (MAGIC_CREATE_PROCESS, "CreateProcess")
    (MAGIC_ZOOM, "Zoom")
    (MAGIC_TIME, "Time")
    (MAGIC_UNK98, "UNK98")
    (MAGIC_RETURN, "Return")
    (MAGIC_UNK100, "UNK100")
    (MAGIC_UNK101, "UNK101")
    (MAGIC_REGISTER_CALLBACK, "RegisterCallback")
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
    (MAGIC_SYSTEM, "System")
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
    (MAGIC_LOAD_TEXTURE_CLIP, "LoadTextureClip")
    (MAGIC_UNK152, "UNK152")
    (MAGIC_UNK153, "UNK153")
    (MAGIC_UNK154, "UNK154")
    (MAGIC_CREATE_SOUND, "CreateSound")
    (MAGIC_SET_LOOP_POINT, "SetLoopPoint")
    (MAGIC_UNK157, "UNK157")
    (MAGIC_UNK158, "UNK158")
    (MAGIC_SET_LOOP, "SetLoop")
    (MAGIC_UNK160, "UNK160")
    (MAGIC_UNK161, "UNK161")
    (MAGIC_UNK162, "UNK162")
    (MAGIC_SET_TEXTBOX_ATTRIBUTES, "SetTextboxAttributes")
    (MAGIC_UNK164, "UNK164")
    (MAGIC_WAIT_TEXT, "WaitText")
    (MAGIC_UNK166, "UNK166")
    (MAGIC_UNK167, "UNK167")
    (MAGIC_UNK168, "UNK168")
    (MAGIC_UNK169, "UNK169")
    (MAGIC_UNK170, "UNK170")
    (MAGIC_UNK171, "UNK171")
    (MAGIC_UNK172, "UNK172")
    (MAGIC_UNK173, "UNK173")
    (MAGIC_UNK174, "UNK174")
    (MAGIC_WRITE_FILE, "WriteFile")
    (MAGIC_UNK176, "UNK176")
    (MAGIC_UNK177, "UNK177")
    (MAGIC_UNK178, "UNK178")
    (MAGIC_UNK179, "UNK179")
    (MAGIC_UNK180, "UNK180")
    (MAGIC_UNK181, "UNK181")
    (MAGIC_UNK182, "UNK182")
    (MAGIC_UNK183, "UNK183")
    (MAGIC_UNK184, "UNK184")
    (MAGIC_UNK185, "UNK185")
    (MAGIC_UNK186, "UNK186")
    (MAGIC_UNK187, "UNK187")
    (MAGIC_UNK188, "UNK188")
    (MAGIC_UNK189, "UNK189")
    (MAGIC_UNK190, "UNK190")
    (MAGIC_UNK191, "UNK191")
    (MAGIC_UNK192, "UNK192")
    (MAGIC_UNK193, "UNK193")
    (MAGIC_UNK194, "UNK194");

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

uint32_t NsbFile::GetSymbol(const std::string& Name, SymbolType Type)
{
    std::map<std::string, uint32_t>::iterator iter;
    switch (Type)
    {
        case SYMBOL_CHAPTER:
            iter = Chapters.find(Name);
            if (iter != Chapters.end())
                return iter->second;
        case SYMBOL_FUNCTION:
            iter = Functions.find(Name);
            if (iter != Functions.end())
                return iter->second;
        case SYMBOL_SCENE:
            iter = Scenes.find(Name);
            if (iter != Scenes.end())
                return iter->second;
        default:
            return NSB_INVALIDE_LINE;
    }
}

/* PRIVATE */

void NsbFile::Read(std::istream* pStream)
{
    uint32_t Entry, Length;
    uint16_t NumParams;
    Line* CurrLine;
    std::locale loc = generator().generate("ja_JP.SHIFT-JIS");

    // Read source code lines
    while (pStream->read((char*)&Entry, sizeof(uint32_t)))
    {
        Entry -= 1; // Start counting at zero
        Source.resize(Source.size() + 1);
        CurrLine = &Source[Entry];
        pStream->read((char*)&CurrLine->Magic, sizeof(uint16_t));
        pStream->read((char*)&NumParams, sizeof(uint16_t));
        CurrLine->Params.reserve(NumParams);

        // Read parameters
        for (uint16_t i = 0; i < NumParams; ++i)
        {
            pStream->read((char*)&Length, sizeof(uint32_t));
            char* String = new char[Length];
            pStream->read(String, Length);
            CurrLine->Params.push_back(to_utf<char>(String, String + Length, loc));
            delete[] String;
        }

        // Map function
        if (CurrLine->Magic == MAGIC_FUNCTION_BEGIN)
            Functions[CurrLine->Params[0].c_str() + strlen("function.")] = Entry;
        // Map chapter
        else if (CurrLine->Magic == MAGIC_CHAPTER_BEGIN)
            Chapters[CurrLine->Params[0].c_str() + strlen("chapter.")] = Entry;
        // Map scene
        else if (CurrLine->Magic == MAGIC_SCENE_BEGIN)
            Scenes[CurrLine->Params[0].c_str() + strlen("scene.")] = Entry;
    }
}
