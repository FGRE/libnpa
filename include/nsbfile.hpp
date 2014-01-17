#ifndef NSB_FILE_HPP
#define NSB_FILE_HPP

#include "scriptfile.hpp"
#include <map>

enum SymbolType
{
    SYMBOL_CHAPTER,
    SYMBOL_SCENE,
    SYMBOL_FUNCTION
};

struct Line
{
    uint16_t Magic;
    std::vector<std::string> Params;
};

class NsbFile : public ScriptFile<Line>
{
public:
    NsbFile(const std::string& Name, char* Data = nullptr, uint32_t Size = 0);

    static bool IsValidMagic(uint16_t Magic);
    static const char* StringifyMagic(uint16_t Magic);
    static uint16_t MagicifyString(const char* String);
    uint32_t GetSymbol(const std::string& Name, SymbolType Type);

private:
    void Read(std::istream* pStream);

    std::map<std::string, uint32_t> Functions;
    std::map<std::string, uint32_t> Chapters;
    std::map<std::string, uint32_t> Scenes;
    std::string Name;
};

#endif
