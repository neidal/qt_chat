#ifndef __FCLOOPER_H_
#define __FCLOOPER_H_

#include "fcdefs.h"

#ifdef _WIN32
#include "win32/looper.h"
#elif defined(__ANDROID__) || defined(__linux__) || defined(_MAC) || defined(__APPLE__)
#include "linux/looper.h"
#endif

#endif //__FCLOOPER_H_
