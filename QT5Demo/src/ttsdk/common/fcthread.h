#ifndef __FCTHREAD_H_
#define __FCTHREAD_H_

#include "fcdefs.h"

#ifdef _WIN32
#include "win32/thread.h"
#elif defined(__ANDROID__) || defined(__linux__) || defined(_MAC) || defined(__APPLE__)
#include "linux/thread.h"
#endif

#endif //__FCTHREAD_H_
