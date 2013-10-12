#include "nsbfile.hpp"

#include <cassert>
#include <fstream>
#include <iostream>

NsbFile::NsbFile(std::string Name, OpenMode Mode)
{
    switch (Mode)
    {
        case NSB_COMPILED:
        case NSB_PARSED:
            break;
        default:
            assert(false);
    }
}

void ReadFromSource(std::string& Name)
{
}

void ReadFromBinary(std::string& Name)
{
    std::ifstream File(Name, std::ios::in | std::ios::binary);
    uint32_t Entry, Magic, Length;
    char* String = nullptr;

    while (File.read((char*)&Entry, sizeof(uint32_t)))
    {
        File.read((char*)&Magic, sizeof(uint32_t));
        switch (Magic)
        {
            case MAGIC_UNK1:
            case MAGIC_UNK2:
            case MAGIC_UNK3:
            case MAGIC_UNK4:
            case MAGIC_UNK5:
            case MAGIC_UNK6:
            case MAGIC_UNK7:
            case MAGIC_UNK8:

            case MAGIC_IF:
            case MAGIC_ENDIF:
            case MAGIC_BEGIN:
            case MAGIC_TEXT:
            case MAGIC_DATA:
                File.read((char*)&Length, sizeof(uint32_t));
                delete[] String;
                String = new char[Length];
                File.read(String, Length);
                break;
            default:
                std::cout << "Unknown magic: " << std::hex << Magic << std::endl;
                assert(false);
        }
    }

    delete[] String;
}
