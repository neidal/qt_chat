#ifndef __FCEVENT_H_
#define __FCEVENT_H_

#include "fcdefs.h"

#ifdef _WIN32
#include "win32/event.h"
#elif defined(__ANDROID__) || defined(__linux__) || defined(_MAC) || defined(__APPLE__)
#include "linux/event.h"
#endif

#endif //__FCEVENT_H_
