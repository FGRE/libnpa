#ifndef SCRIPT_FILE_HPP
#define SCRIPT_FILE_HPP

#include <string>
#include <cstdint>

class ScriptFile
{
public:
    std::string GetName() const;

protected:
    void Open(const std::string& Name, char* Data = nullptr, uint32_t Size = 0);

private:
    virtual void Read(std::istream* pStream) = 0;
    std::string Name;
};

#endif
