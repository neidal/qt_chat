#ifndef __FCSYSTEM_H_
#define __FCSYSTEM_H_

#include "fcdefs.h"

#ifdef _WIN32
#include "win32/system.h"
#elif defined(__ANDROID__) || defined(__linux__) || defined(_MAC) || defined(__APPLE__)
#include "linux/system.h"
#endif

#endif //__FCSYSTEM_H_
