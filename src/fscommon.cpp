#include "fscommon.hpp"
#include <fstream>
#include <boost/filesystem.hpp>

namespace fs
{

char* ReadFile(const std::string& Filename, uint32_t& Size)
{
    std::ifstream File(Filename, std::ios::binary);
    if (!File)
        return nullptr;

    File.seekg(0, std::ios::end);
    Size = File.tellg();
    char* pData = new char[Size];
    File.seekg(0, std::ios::beg);
    File.read(pData, Size);
    return pData;
}

void WriteFileDirectory(const std::string& Filename, const char* pData, uint32_t Size)
{
    using namespace boost::filesystem;

    // Create directories
    std::string Path = Filename;
    if (char* delim = strchr((char*)Filename.c_str(), '/'))
    {
        do
        {
            *delim = 0;
            if (!exists(path(Path)))
                create_directory(path(Path));
            *delim = '/';
        } while ((delim = strchr(delim + 1, '/')));
    }

    WriteFile(Filename, pData, Size);
}

void WriteFile(const std::string& Filename, const char* pData, uint32_t Size)
{
    std::ofstream File(Filename, std::ios::binary);
    File.write(pData, Size);
}

}
