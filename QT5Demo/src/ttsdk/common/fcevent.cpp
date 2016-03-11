#include "fcevent.h"

#ifdef _WIN32
#include "win32/event.cpp"
#elif defined(__ANDROID__) || defined(__linux__) || defined(_MAC)
#include "linux/event.cpp"
#endif
