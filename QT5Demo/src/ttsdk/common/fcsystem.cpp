#include "fcsystem.h"

#ifdef _WIN32
#include "win32/system.cpp"
#elif defined(__ANDROID__) || defined(__linux__) || defined(_MAC)
#include "linux/system.cpp"
#endif
