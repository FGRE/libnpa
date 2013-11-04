#ifndef NSB_FILE_HPP
#define NSB_FILE_HPP

#include "scriptfile.hpp"

#include <map>

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
    uint32_t GetFunctionLine(const char* Name) const;

private:
    void Read(std::istream* pStream);

    std::map<std::string, uint32_t> Functions;
    std::string Name;
};

#endif
