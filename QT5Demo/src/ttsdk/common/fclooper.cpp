#include "fclooper.h"

#ifdef _WIN32
#include "win32/looper.cpp"
#elif defined(__ANDROID__) || defined(__linux__) || defined(_MAC)
#include "linux/looper.cpp"
#endif
