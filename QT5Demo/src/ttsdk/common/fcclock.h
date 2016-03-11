#ifndef __FCLOCK_H_
#define __FCLOCK_H_

#include "fcdefs.h"

#ifdef _WIN32
#include "win32/lock.h"
#elif defined(__ANDROID__) || defined(__linux__) || defined(_MAC) || defined(__APPLE__)
#include "linux/lock.h"
#endif

#endif //__FCLOCK_H_
