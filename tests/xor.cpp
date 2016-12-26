
#include <cassert>
#include "npafile.hpp"

class EmptyNpaFile : NpaFile
{
public:
    EmptyNpaFile(const std::string& empty);
    ~EmptyNpaFile();
    void testXOR();
};

EmptyNpaFile::EmptyNpaFile(const std::string& empty) : NpaFile(empty)
{
}

EmptyNpaFile::~EmptyNpaFile()
{
}

void EmptyNpaFile::testXOR()
{
    char start[15] = "El Psy Congroo";
    char work[15];
    strcpy(work, start);
    Encrypt(work, 14);
    Decrypt(work, 14);

    assert(strcmp(start, work) == 0);
    printf("in: '%s', out: '%s'\n", start, work);
}

int main(int argc, char** argv)
{

    EmptyNpaFile empty("");
    empty.testXOR();

    return 0;
}
