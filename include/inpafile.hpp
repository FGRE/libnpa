#ifndef I_NPA_FILE_HPP
#define I_NPA_FILE_HPP

#include "npafile.hpp"
#include <map>

// GStreamer requires g_malloc instead of ::operator new because it deallocates memory with g_free
void* DefaultAlloc(size_t Size);

class INpaFile : public NpaFile
{
public:
    struct Entry
    {
        virtual ~Entry() {}
        uint32_t Size;
        uint32_t Offset;
    };
    typedef std::map<std::string, Entry*> NpaRegistry;
    typedef NpaRegistry::iterator NpaIterator;

    INpaFile(const std::string& Filename) : NpaFile(Filename) { }
    virtual ~INpaFile() = 0;

    virtual bool IsDirectory(NpaIterator iter) = 0;
    char* ReadFile(const std::string& Filename, uint32_t& Size);
    virtual char* ReadFile(NpaIterator iter);
    char* ReadFile(NpaIterator iter, uint32_t Offset, uint32_t Size, void *(*Alloc)(size_t) = DefaultAlloc);
    NpaIterator Begin() { return Registry.begin(); }
    NpaIterator End() { return Registry.end(); }
    NpaIterator FindFile(const std::string& Filename) { return Registry.find(Filename); }
    uint32_t GetFileSize(NpaIterator iter) { return iter->second->Size; }

protected:
    virtual char* ReadData(uint32_t GlobalOffset, uint32_t LocalOffset, uint32_t Size, void *(*Alloc)(size_t)) = 0;

    NpaRegistry Registry;
};

#endif
