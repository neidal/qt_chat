#ifndef __FCLIMITS_H_
#define __FCLIMITS_H_

#ifdef _WIN32
#include <limits.h>
#elif defined(__APPLE__)
#include <limits.h>
#elif defined(__ANDROID__) || defined(__linux__)
#include <linux/limits.h>
#endif

#endif //__FCLIMITS_H_
