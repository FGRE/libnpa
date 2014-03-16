#ifndef SCRIPT_FILE_HPP
#define SCRIPT_FILE_HPP

#include <vector>
#include <string>
#include <map>
#include <cstdint>
#define NSB_INVALIDE_LINE ~(uint32_t)0

struct Line
{
    uint16_t Magic;
    std::vector<std::string> Params;
};

class ScriptFile
{
public:
    ScriptFile(std::string Name, char* NsbData, uint32_t NsbSize, char* MapData, uint32_t MapSize);
    ScriptFile(std::string Name);

    void SetSourceIter(uint32_t NewIter) { SourceIter = NewIter; }
    uint32_t GetNextLineEntry() const { return SourceIter; }
    Line* GetNextLine() { return SourceIter < Source.size() ? &Source[SourceIter++] : nullptr; }
    Line* GetPrevLine() { SourceIter -= 2; return GetNextLine(); }
    uint32_t GetSymbol(const std::string& Symbol);
    const std::string& GetName() const { return Name; }

private:
    void Open(char* NsbData, uint32_t NsbSize, char* MapData, uint32_t MapSize);
    void Read(char** Src, void* Dest, uint32_t Size);

    std::map<std::string, uint32_t> Symbols;
    std::vector<Line> Source;
    uint32_t SourceIter;
    const std::string Name;
};

#endif
