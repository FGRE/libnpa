#include "fscommon.hpp"
#include <fstream>
#include <boost/filesystem.hpp>
using namespace boost::filesystem;

namespace fs
{

char* ReadFile(const string& Filename, uint32_t& Size)
{
    ifstream File(Filename, ios::binary);
    if (!File)
        return nullptr;

    File.seekg(0, ios::end);
    Size = File.tellg();
    char* pData = new char[Size];
    File.seekg(0, ios::beg);
    File.read(pData, Size);
    return pData;
}

void WriteFileDirectory(const string& Filename, const char* pData, uint32_t Size)
{
    // Create directories
    string Path = Filename;
    if (char* delim = strchr((char*)Filename.c_str(), '/'))
    {
        do
        {
            *delim = 0;
            if (!Exists(Path))
                CreateDirectory(Path);
            *delim = '/';
        } while ((delim = strchr(delim + 1, '/')));
    }

    WriteFile(Filename, pData, Size);
}

void WriteFile(const string& Filename, const char* pData, uint32_t Size)
{
    ofstream File(Filename, ios::binary);
    File.write(pData, Size);
}

bool Exists(const string& Path)
{
    return exists(path(Path));
}

void CreateDirectory(const string& Path)
{
    create_directory(path(Path));
}

void DeleteFile(const string& Path)
{
    remove(path(Path));
}

void DeleteDirectory(const string& Path)
{
    remove_all(path(Path));
}

}
