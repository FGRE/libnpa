#ifndef SCRIPT_FILE_HPP
#define SCRIPT_FILE_HPP

#include <vector>
#include <string>
#include <map>
#include <list>
#include <cstdint>
using namespace std;
#define NSB_INVALIDE_LINE ~(uint32_t)0

namespace Npa
{
    class Buffer;
}

struct Line
{
    uint16_t Magic;
    vector<string> Params;
};

class ScriptFile
{
public:
    enum FileType
    {
        NSB,
        NSS
    };

    ScriptFile(const string& Name, const char* NssData, uint32_t NssSize);
    ScriptFile(const string& Name, char* NsbData, uint32_t NsbSize, char* MapData, uint32_t MapSize);
    ScriptFile(const string& Name, FileType Type);

    const map<string, uint32_t> GetSymbols() { return Symbols; }
    const list<string>& GetIncludes() { return Includes; }
    uint32_t GetSymbol(const string& Symbol);
    const string& GetName() const { return Name; }
    Line* GetLine(uint32_t Entry) { return Entry < Source.size() ? &Source[Entry] : nullptr; }

private:
    void OpenNss(const string& Name);
    void OpenNsb(string Name);
    void ReadNss(const char* NssData, uint32_t NssSize);
    void ReadNsb(Npa::Buffer& NsbData, Npa::Buffer& MapData);

    map<string, uint32_t> Symbols;
    list<string> Includes;
    vector<Line> Source;
    const string Name;
};

namespace Nss
{
    void Compile(const char* pBuffer, uint32_t Length, Npa::Buffer* NsbBuffer, Npa::Buffer* MapBuffer);
}

#endif
