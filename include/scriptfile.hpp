#ifndef SCRIPT_FILE_HPP
#define SCRIPT_FILE_HPP

#include <string>
#include <cstdint>
#include <vector>
#include <fstream>

struct membuf : std::streambuf
{
    membuf(char* Data, uint32_t Size) { this->setg(Data, Data, Data + Size); }
};

template <class L>
class ScriptFile
{
public:
    ScriptFile();

    std::string GetName() const;
    void SetSourceIter(uint32_t NewIter);
    uint32_t GetNextLineEntry() const;
    L* GetNextLine();

protected:
    void Open(const std::string& Name, char* Data = nullptr, uint32_t Size = 0);

    std::vector<L> Source;
    uint32_t SourceIter;

private:
    virtual void Read(std::istream* pStream) = 0;
    std::string Name;
};

template <class L>
ScriptFile<L>::ScriptFile() :
SourceIter(0)
{
}

template <class L>
void ScriptFile<L>::Open(const std::string& Name, char* Data, uint32_t Size)
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

template <class L>
std::string ScriptFile<L>::GetName() const
{
    return Name;
}

template <class L>
void ScriptFile<L>::SetSourceIter(uint32_t NewIter)
{
    SourceIter = NewIter;
}

template <class L>
uint32_t ScriptFile<L>::GetNextLineEntry() const
{
    return SourceIter;
}

template <class L>
L* ScriptFile<L>::GetNextLine()
{
    return SourceIter < Source.size() ? &Source[SourceIter++] : nullptr;
}

#endif
