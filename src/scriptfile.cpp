#include "scriptfile.hpp"

#include <cassert>
#include <fstream>

struct membuf : std::streambuf
{
    membuf(char* Data, uint32_t Size) { this->setg(Data, Data, Data + Size); }
};

void ScriptFile::Open(const std::string& Name, char* Data, uint32_t Size)
{
    std::istream* pStream;
    membuf* pBuf;

    this->Name = Name;

    if (Data && Size)
    {
        pBuf = new membuf(Data, Size);
        pStream = new std::istream(pBuf);
    }
    else
    {
        pBuf = nullptr;
        pStream = new std::ifstream(Name, std::ios::in | std::ios::binary);
    }

    Read(pStream);
    delete pStream;
    delete pBuf;
}

std::string ScriptFile::GetName() const
{
    return Name;
}
