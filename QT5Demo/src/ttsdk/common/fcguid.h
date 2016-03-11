#ifndef __FCGUID_H_
#define __FCGUID_H_

#include "fcdefs.h"
#include <string.h>

#ifdef _WIN32
#include <guiddef.h>
#else

/* GUID定义 */
typedef struct _GUID {
	unsigned int  Data1;
	unsigned short Data2;
	unsigned short Data3;
	unsigned char  Data4[ 8 ];
} GUID;

#if !defined (_SYS_GUID_OPERATORS_)
#define _SYS_GUID_OPERATORS_

// Faster (but makes code fatter) inline version...use sparingly
#ifdef __cplusplus
fcinline int InlineIsEqualGUID(const GUID& rguid1, const GUID& rguid2)
{
   return (
      ((unsigned int *) &rguid1)[0] == ((unsigned int *) &rguid2)[0] &&
      ((unsigned int *) &rguid1)[1] == ((unsigned int *) &rguid2)[1] &&
      ((unsigned int *) &rguid1)[2] == ((unsigned int *) &rguid2)[2] &&
      ((unsigned int *) &rguid1)[3] == ((unsigned int *) &rguid2)[3]);
}
fcinline int IsEqualGUID(const GUID& rguid1, const GUID& rguid2)
{
    return !memcmp(&rguid1, &rguid2, sizeof(GUID));
}

#else   // ! __cplusplus
#define InlineIsEqualGUID(rguid1, rguid2)  \
       (((unsigned int *) rguid1)[0] == ((unsigned int *) rguid2)[0] &&   \
        ((unsigned int *) rguid1)[1] == ((unsigned int *) rguid2)[1] &&    \
        ((unsigned int *) rguid1)[2] == ((unsigned int *) rguid2)[2] &&    \
        ((unsigned int *) rguid1)[3] == ((unsigned int *) rguid2)[3])

#define IsEqualGUID(rguid1, rguid2) (!memcmp(rguid1, rguid2, sizeof(GUID)))

#endif  // __cplusplus

#ifdef __INLINE_ISEQUAL_GUID
#undef IsEqualGUID
#define IsEqualGUID(rguid1, rguid2) InlineIsEqualGUID(rguid1, rguid2)
#endif

#if !defined _SYS_GUID_OPERATOR_EQ_ && !defined _NO_SYS_GUID_OPERATOR_EQ_
#define _SYS_GUID_OPERATOR_EQ_
// A couple of C++ helpers

#ifdef __cplusplus
fcinline bool operator==(const GUID& guidOne, const GUID& guidOther)
{
	return IsEqualGUID(guidOne,guidOther);
}

fcinline bool operator!=(const GUID& guidOne, const GUID& guidOther)
{
	return (bool)(!(guidOne == guidOther));
}
#endif
#endif  // _SYS_GUID_OPERATOR_EQ_
#endif  // _SYS_GUID_OPERATORS_

#endif //!WIN32

#ifndef ZeroGUID
#define ZeroGUID(p) memset(p,0,sizeof(GUID))
#endif //ZeroGUID

#endif // __FCGUID_H_
