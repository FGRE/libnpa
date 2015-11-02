// msvc doesn't have unistd.h, the equivalent is io.h
// thus, a dummy unistd.h that includes io.h is used
// if msvc is detected
#include <io.h>
