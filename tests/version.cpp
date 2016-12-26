
#include <string>
#include <cassert>
#include "npaversion.hpp"

int main(int argc, char** argv)
{

    assert(NPA_VERSION_MAJOR >= 0);
    assert(NPA_VERSION_MINOR >= 0);
    assert(NPA_VERSION_PATCH >= 0);
    assert(NPA_VERSION_MAJOR+NPA_VERSION_MINOR+NPA_VERSION_PATCH > 0);

    printf("libnpa version: %s\n", NPA_VERSION);

    return 0;
}
