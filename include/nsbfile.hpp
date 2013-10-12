#ifndef NSB_FILE_HPP
#define NSB_FILE_HPP

#include <string>

enum OpenMode
{
    NSB_COMPILED    = 1,
    NSB_PARSED      = 2
};

class NsbFile
{
public:
	NsbFile(std::string Name, OpenMode Mode);
};

#endif
