#ifndef I_NPA_FILE_HPP
#define I_NPA_FILE_HPP

#include "npafile.hpp"
#include <map>

// GStreamer requires g_malloc instead of ::operator new because it deallocates memory with g_free
void* DefaultAlloc(size_t Size);

template <class Entry>
class INpaFile : public NpaFile
{
public:
    typedef typename std::map<std::string, Entry>::iterator NpaIterator;

    INpaFile(const std::string& Filename) : NpaFile(Filename) { }
    virtual ~INpaFile() = 0;

    char* ReadFile(const std::string& Filename, uint32_t& Size);
    virtual char* ReadFile(NpaIterator iter);
    char* ReadFile(NpaIterator iter, uint32_t Offset, uint32_t Size, void *(*Alloc)(size_t) = DefaultAlloc);
    NpaIterator Begin() { return Registry.begin(); }
    NpaIterator End() { return Registry.end(); }
    NpaIterator FindFile(const std::string& Filename) { return Registry.find(Filename); }
    virtual uint32_t GetFileSize(NpaIterator iter) = 0;

protected:
    virtual char* ReadData(uint32_t GlobalOffset, uint32_t LocalOffset, uint32_t Size, void *(*Alloc)(size_t)) = 0;

    std::map<std::string, Entry> Registry;
};

template <class Entry>
INpaFile<Entry>::~INpaFile()
{
}

template <class Entry>
char* INpaFile<Entry>::ReadFile(const std::string& Filename, uint32_t& Size)
{
    auto iter = FindFile(Filename);
    if (iter == End())
        return nullptr;

    char* pData = ReadFile(iter);
    Size = GetFileSize(iter);
    return pData;
}

template <class Entry>
char* INpaFile<Entry>::ReadFile(NpaIterator iter)
{
    return ReadFile(iter, 0, GetFileSize(iter));
}

template <class Entry>
char* INpaFile<Entry>::ReadFile(NpaIterator iter, uint32_t Offset, uint32_t Size, void *(*Alloc)(size_t))
{
    return ReadData(iter->second.Offset, Offset, Size, Alloc);
}

#endif
