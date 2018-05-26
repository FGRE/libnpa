#include "nsbmagic.hpp"
#include <boost/assign/list_of.hpp>
#include <boost/bimap.hpp>

typedef boost::bimap<uint16_t, std::string> LookupTable;

static const LookupTable MagicStrings = boost::assign::list_of<LookupTable::value_type>
    (MAGIC_FWN_UNK, "FUWANOVEL")

    (MAGIC_PARSE_TEXT, "ParseText")
    (MAGIC_LITERAL, "Literal")
    (MAGIC_CHAPTER_DECLARATION, "ChapterDeclaration")
    (MAGIC_IF, "If")
    (MAGIC_JUMP, "Jump")
    (MAGIC_CALL_FUNCTION, "CallFunction")
    (MAGIC_CMP_LOGICAL_AND, "CmpLogicalAnd")
    (MAGIC_CMP_LOGICAL_OR, "CmpLogicalOr")
    (MAGIC_SCOPE_BEGIN, "ScopeBegin")
    (MAGIC_SCOPE_END, "ScopeEnd")
    (MAGIC_MODULE_FILE_NAME, "ModuleFileName")
    (MAGIC_CLEAR_PARAMS, "ClearParams")
    (MAGIC_RESET, "Reset")
    (MAGIC_LOGICAL_NOT_EQUAL, "LogicalNotEqual")
    (MAGIC_ADD_EXPRESSION, "AddExpression")
    (MAGIC_ASSIGN, "Assign")
    (MAGIC_VARIABLE, "Variable")
    (MAGIC_END_CHAPTER, "EndChapter")
    (MAGIC_SCENE_DECLARATION, "SceneDeclaration")
    (MAGIC_CALL_SCENE, "CallScene")
    (MAGIC_END_SCENE, "EndScene")
    (MAGIC_MOVE, "Move")
    (MAGIC_REQUEST, "Request")
    (MAGIC_DELETE, "Delete")
    (MAGIC_FADE, "Fade")
    (MAGIC_WAIT, "Wait")
    (MAGIC_WAIT_KEY, "WaitKey")
    (MAGIC_AT_EXPRESSION, "AtExpression")
    (MAGIC_NEGA_EXPRESSION, "NegaExpression")
    (MAGIC_SET_VOLUME, "SetVolume")
    (MAGIC_SHAKE, "Shake")
    (MAGIC_SUB_SCRIPT, "SubScript")
    (MAGIC_UNK27, "UNK27")
    (MAGIC_CREATE_WINDOW, "CreateWindow")
    (MAGIC_SET_ALIAS, "SetAlias")
    (MAGIC_CREATE_MOVIE, "CreateMovie")
    (MAGIC_SET_VERTEX, "SetVertex")
    (MAGIC_SET_SHADE, "SetShade")
    (MAGIC_CREATE_RENDER_TEXTURE, "CreateRenderTexture")
    (MAGIC_DRAW_TO_TEXTURE, "DrawToTexture")
    (MAGIC_CMP_EQUAL, "CmpEqual")
    (MAGIC_FUNCTION_DECLARATION, "FunctionDeclaration")
    (MAGIC_END_FUNCTION, "EndFunction")
    (MAGIC_UNK38, "UNK38")
    (MAGIC_UNK39, "UNK39")
    (MAGIC_VARIABLE_VALUE, "VariableValue")
    (MAGIC_CREATE_TEXTURE, "CreateTexture")
    (MAGIC_IMAGE_HORIZON, "ImageHorizon")
    (MAGIC_DIV_EXPRESSION, "DivExpression")
    (MAGIC_MUL_EXPRESSION, "MulExpression")
    (MAGIC_LOGICAL_NOT, "LogicalNot")
    (MAGIC_WHILE, "While")
    (MAGIC_REMAIN_TIME, "RemainTime")
    (MAGIC_BREAK, "Break")
    (MAGIC_LOGICAL_LESS_EQUAL, "LogicalLessEqual")
    (MAGIC_CREATE_EFFECT, "CreateEffect")
    (MAGIC_CREATE_SURFACE, "CreateSurface")
    (MAGIC_SET_SURFACE, "SetSurface")
    (MAGIC_ALLOW_PHONE_CALL, "AllowPhoneCall")
    (MAGIC_INCREMENT, "Increment")
    (MAGIC_CMP_GREATER, "CmpGreater")
    (MAGIC_DRAW_TRANSITION, "DrawTransition")
    (MAGIC_LOGICAL_GREATER_EQUAL, "LogicalGreaterEqual")
    (MAGIC_SUB_EXPRESSION, "SubExpression")
    (MAGIC_UNK59, "UNK59")
    (MAGIC_UNK60, "UNK60")
    (MAGIC_ADD_ASSIGN, "AddAssign")
    (MAGIC_UNK62, "UNK62")
    (MAGIC_SUB_ASSIGN, "SubAssign")
    (MAGIC_CMP_LESS, "CmpLess")
    (MAGIC_WHILE_END, "WhileEnd")
    (MAGIC_LOCK_VIDEO, "LockVideo")
    (MAGIC_LOAD_FONT, "LoadFont")
    (MAGIC_LOAD_IMAGE, "LoadImage")
    (MAGIC_ARRAY, "Array")
    (MAGIC_ASSOC_ARRAY, "AssocArray")
    (MAGIC_CREATE_CHOICE, "CreateChoice")
    (MAGIC_IMAGE_VERTICAL, "ImageVertical")
    (MAGIC_COUNT, "Count")
    (MAGIC_SET_FONT, "SetFont")
    (MAGIC_CREATE_TEXT, "CreateText")
    (MAGIC_CREATE_COLOR, "CreateColor")
    (MAGIC_CREATE_NAME, "CreateName")
    (MAGIC_CREATE_SCROLLBAR, "CreateScrollbar")
    (MAGIC_SET_SCROLL_SPEED, "SetScrollSpeed")
    (MAGIC_SET_SCROLLBAR_WHEEL_AREA, "SetScrollbarWheelArea")
    (MAGIC_MOD_EXPRESSION, "ModExpression")
    (MAGIC_INTEGER, "Integer")
    (MAGIC_STRING, "String")
    (MAGIC_SET_SCROLLBAR_VALUE, "SetScrollbarValue")
    (MAGIC_SELECT, "Select")
    (MAGIC_CALL_CHAPTER, "CallChapter")
    (MAGIC_CASE, "Case")
    (MAGIC_CASE_END, "CaseEnd")
    (MAGIC_SELECT_END, "SelectEnd")
    (MAGIC_SELECT_BREAK_END, "SelectBreakEnd")
    (MAGIC_CURSOR_POSITION, "CursorPosition")
    (MAGIC_MOVE_CURSOR, "MoveCursor")
    (MAGIC_SCROLLBAR_VALUE, "ScrollbarValue")
    (MAGIC_SET_ACHIEVEMENT, "SetAchievement")
    (MAGIC_CREATE_PROCESS, "CreateProcess")
    (MAGIC_ZOOM, "Zoom")
    (MAGIC_TIME, "Time")
    (MAGIC_SET_NEXT_FOCUS, "SetNextFocus")
    (MAGIC_RETURN, "Return")
    (MAGIC_UNK100, "UNK100")
    (MAGIC_UNK101, "UNK101")
    (MAGIC_SET_SHORTCUT, "SetShortcut")
    (MAGIC_SAVE, "Save")
    (MAGIC_DELETE_SAVE_FILE, "DeleteSaveFile")
    (MAGIC_DURATION_TIME, "DurationTime")
    (MAGIC_CLEAR_SCORE, "ClearScore")
    (MAGIC_CLEAR_BACKLOG, "ClearBacklog")
    (MAGIC_EXIST_SAVE, "ExistSave")
    (MAGIC_WAIT_ACTION, "WaitAction")
    (MAGIC_DECREMENT, "Decrement")
    (MAGIC_MOVIE_PLAY, "MoviePlay")
    (MAGIC_SET_STREAM, "SetStream")
    (MAGIC_WAIT_PLAY, "WaitPlay")
    (MAGIC_SYSTEM, "System")
    (MAGIC_UNK115, "UNK115")
    (MAGIC_UNK116, "UNK116")
    (MAGIC_UNK117, "UNK117")
    (MAGIC_WAIT_FINISH_ATTACH_CG_VIEW, "WaitFinishAttachCgView")
    (MAGIC_UNK119, "UNK119")
    (MAGIC_WAIT_FINISH_SEND_MAIL_EDIT, "WaitFinishSendMailEdit")
    (MAGIC_WAIT_FINISH_MAIL_VIEW, "WaitFinishMailView")
    (MAGIC_UNK122, "UNK122")
    (MAGIC_UNK123, "UNK123")
    (MAGIC_UNK124, "UNK124")
    (MAGIC_UNK125, "UNK125")
    (MAGIC_UNK126, "UNK126")
    (MAGIC_UNK127, "UNK127")
    (MAGIC_UNK128, "UNK128")
    (MAGIC_UNK129, "UNK129")
    (MAGIC_SEND_MAIL_EDIT, "SendMailEdit")
    (MAGIC_RANDOM, "Random")
    (MAGIC_UNK132, "UNK132")
    (MAGIC_START_ATCH, "StartAtch")
    (MAGIC_WAIT_ATCH, "WaitAtch")
    (MAGIC_DELETE_ATCH, "DeleteAtch")
    (MAGIC_UNK136, "UNK136")
    (MAGIC_UNK137, "UNK137")
    (MAGIC_UNK138, "UNK138")
    (MAGIC_UNK139, "UNK139")
    (MAGIC_UNK140, "UNK140")
    (MAGIC_UNK141, "UNK141")
    (MAGIC_UNK142, "UNK142")
    (MAGIC_UNK143, "UNK143")
    (MAGIC_UNK144, "UNK144")
    (MAGIC_TIME_LEAP_INIT, "TimeLeapInit")
    (MAGIC_UNK146, "UNK146")
    (MAGIC_DISP_ATTACH_CG, "DispAttachCg")
    (MAGIC_ERASE_ATTACH_CG, "EraseAttachCg")
    (MAGIC_UNK149, "UNK149")
    (MAGIC_UNK150, "UNK150")
    (MAGIC_CREATE_CLIP_TEXTURE, "CreateClipTexture")
    (MAGIC_CREATE_STENCIL, "CreateStencil")
    (MAGIC_CREATE_MASK, "CreateMask")
    (MAGIC_UNK154, "UNK154")
    (MAGIC_CREATE_SOUND, "CreateSound")
    (MAGIC_SET_LOOP_POINT, "SetLoopPoint")
    (MAGIC_SET_FREQUENCY, "SetFrequency")
    (MAGIC_SET_PAN, "SetPan")
    (MAGIC_SET_LOOP, "SetLoop")
    (MAGIC_UNK160, "UNK160")
    (MAGIC_CONQUEST, "Conquest")
    (MAGIC_STR_STR, "StrStr")
    (MAGIC_SET_TEXTBOX_ATTRIBUTES, "SetTextboxAttributes")
    (MAGIC_LOAD_TEXT, "LoadText")
    (MAGIC_WAIT_TEXT, "WaitText")
    (MAGIC_SET_BACKLOG, "SetBacklog")
    (MAGIC_ESCAPE, "Escape")
    (MAGIC_PASSAGE_TIME, "PassageTime")
    (MAGIC_SET_COLOR, "SetColor")
    (MAGIC_WAIT_FADE, "WaitFade")
    (MAGIC_POSITION, "Position")
    (MAGIC_SOUND_AMPLITUDE, "SoundAmplitude")
    (MAGIC_READ_FILE, "ReadFile")
    (MAGIC_DATE_TIME, "DateTime")
    (MAGIC_WRITE_FILE, "WriteFile")
    (MAGIC_LOAD, "Load")
    (MAGIC_MESSAGE, "Message")
    (MAGIC_UNK178, "UNK178")
    (MAGIC_UNK179, "UNK179")
    (MAGIC_UNK180, "UNK180")
    (MAGIC_CREATE_BACKLOG, "CreateBacklog")
    (MAGIC_SET_BACKLOG_SCROLLBAR, "SetBacklogScrollbar")
    (MAGIC_UNK183, "UNK183")
    (MAGIC_ENABLE_BACKLOG, "EnableBacklog")
    (MAGIC_UNK185, "UNK185")
    (MAGIC_EXIT, "Exit")
    (MAGIC_UNK187, "UNK187")
    (MAGIC_UNK188, "UNK188")
    (MAGIC_ROTATE, "Rotate")
    (MAGIC_SET_TONE, "SetTone")
    (MAGIC_SET_PRIO, "SetPrio")
    (MAGIC_UNK192, "UNK192")
    (MAGIC_UNK193, "UNK193")
    (MAGIC_UNK194, "UNK194");

namespace Nsb
{
    bool IsValidMagic(uint16_t Magic)
    {
        return MagicStrings.left.find(Magic) != MagicStrings.left.end();
    }

    const char* StringifyMagic(uint16_t Magic)
    {
        auto iter = MagicStrings.left.find(Magic);
        if (iter != MagicStrings.left.end())
            return iter->second.c_str();
        return nullptr;
    }

    uint16_t MagicifyString(const char* String)
    {
        auto iter = MagicStrings.right.find(String);
        if (iter != MagicStrings.right.end())
            return iter->second;
        return 0;
    }
}
