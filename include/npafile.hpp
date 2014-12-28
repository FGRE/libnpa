#ifndef NPA_FILE_HPP
#define NPA_FILE_HPP

#include <string>
#include <boost/locale.hpp>
using namespace std;

class NpaFile
{
public:
    NpaFile(const string& Name);
    virtual ~NpaFile() = 0;

    static void SetLocale(const char* LocaleStr);
    static string ToUtf8(const char* pBuff, uint32_t Size);
    static string ToUtf8(const string& String);
    static string ToUtf8(const char* pText);
    static string FromUtf8(const string& String);
    static char* Decrypt(char* pBuff, uint32_t Size, uint32_t Offset = 0);
    static char* Encrypt(char* pBuff, uint32_t Size);

protected:
    string Name;
private:
    static locale Locale;
};

#endif
