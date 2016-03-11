#ifndef _FCHASHMAP_H_
#define _FCHASHMAP_H_

#include "fctypes.h"

#ifndef _WIN32
#define GCC_VERSION (__GUNC__ * 10000 + __GUNC_MINOR__ * 100 + __GUNC_PATCHLEVEL__)

#if (GCC_VERSION < 40300) || !defined(__GXX_EXPERIMENTAL_CXX0X__)
#if defined(__ANDROID__) && !defined(_GLIBCXX_PERMIT_BACKWARD_HASH)
#define _GLIBCXX_PERMIT_BACKWARD_HASH
#endif
#ifdef __DEPRECATED
#undef __DEPRECATED
#include <ext/hash_map>
#define __DEPRECATED
#else
#include <ext/hash_map>
#endif

#define _hash_map        __gnu_cxx::hash_map //hash_map

namespace __gnu_cxx
{
	template<>
	struct hash < long long >
	{
		size_t operator()(long long __x) const
		{
			return __x;
		}
	};

	template<>
	struct hash < unsigned long long >
	{
		size_t operator()(unsigned long long __x) const
		{
			return __x;
		}
	};
} //__gnu_cxx

#else
#include <tr1/unordered_map>
#define _hash_map        std::tr1::unordered_map //hash_map
#endif

#else
#include <ext/hash_map>
#define _hash_map        std::hash_map
#endif //!_WIN32

#endif // _FCHASHMAP_H_
