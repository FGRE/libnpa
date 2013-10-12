#include "nsbfile.hpp"

#include <cassert>

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
