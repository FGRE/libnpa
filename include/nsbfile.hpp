#ifndef NSB_FILE_HPP
#define NSB_FILE_HPP

#include "scriptfile.hpp"

#include <vector>
#include <map>

struct Line
{
    uint16_t Magic;
    std::vector<std::string> Params;
};

class NsbFile : public ScriptFile
{
public:
    NsbFile(const std::string& Name, char* Data = nullptr, uint32_t Size = 0);

    static bool IsValidMagic(uint16_t Magic);
    static const char* StringifyMagic(uint16_t Magic);
    static uint16_t MagicifyString(const char* String);
    Line* GetNextLine();
    uint32_t GetFunctionLine(const char* Name) const;
    void SetSourceIter(uint32_t NewIter);
    uint32_t GetNextLineEntry() const;

private:
    void Read(std::istream* pStream);

    uint32_t SourceIter;
    std::vector<Line> Source;
    std::map<const char*, uint32_t> Functions;
    std::string Name;
};

#endif
