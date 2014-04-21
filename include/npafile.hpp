#ifndef NPA_FILE_HPP
#define NPA_FILE_HPP

#include <string>
#include <boost/locale.hpp>

class NpaFile
{
public:
    NpaFile(const std::string& Name);
    virtual ~NpaFile() = 0;

    static void SetLocale(const char* LocaleStr);
    static std::string ToUtf8(const std::string& String);
    static std::string FromUtf8(const std::string& String);
protected:
    void Decrypt(char* pBuff, uint32_t Size, uint32_t Offset = 0);
    char* Encrypt(char* pBuff, uint32_t Size);

    std::string Name;
private:
    static std::locale Locale;
};

#endif
