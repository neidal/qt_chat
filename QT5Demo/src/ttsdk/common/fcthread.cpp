#include "fcthread.h"

#ifdef _WIN32
#include "win32/thread.cpp"
#elif defined(__ANDROID__) || defined(__linux__) || defined(_MAC)
#include "linux/thread.cpp"
#endif
