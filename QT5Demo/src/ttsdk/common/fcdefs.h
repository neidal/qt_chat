#ifndef __FCDEFS_H_
#define __FCDEFS_H_

#include "fctypes.h"

#include <stdio.h>
#include <stdlib.h>

#ifndef fcinline
#define fcinline inline
#endif

#ifndef __fcinline
#define __fcinline __inline
#endif

/* Try to get PRIx64 from inttypes.h, but if it's not defined, fall back to
* llx, which is the format str for "long long" - this is a 64-bit
* integral type on many systems. */
#ifndef PRIx64
#define PRIx64 "llx"
#endif  /* !PRIx64 */

#ifndef TEXT
#define TEXT(quote) __TEXT(quote)
#endif

#ifndef INVALID_HANDLE_VALUE
#define INVALID_HANDLE_VALUE  ((HANDLE)((LONG_PTR)-1))
#endif  //INVALID_HANDLE_VALUE

#ifndef INFINITE
#define INFINITE ((ULONG)-1)
#endif


//////////////////////////////////////////////////////////////////////////
// Defines MAX_PATH
//#ifndef _WIN32
#ifndef _MAX_PATH
#include "fclimits.h"
#define _MAX_PATH (PATH_MAX + 1)
#endif
//#endif

#ifndef MAX_PATH
#define MAX_PATH    _MAX_PATH
#endif

#ifndef NAME_MAX
#define NAME_MAX 255
#endif // !NAME_MAX

#ifndef MAX_FILENAME
#define MAX_FILENAME NAME_MAX
#endif


#define MINCHAR     0x80
#define MAXCHAR     0x7f
#define MINSHORT    0x8000
#define MAXSHORT    0x7fff
#define MINLONG     0x80000000
#define MAXLONG     0x7fffffff
#define MAXBYTE     0xff
#define MAXWORD     0xffff
#define MAXDWORD    0xffffffff

#ifndef NULL
#ifdef __cplusplus
#define NULL    0
#else
#define NULL    ((void *)0)
#endif
#endif

#ifndef FALSE
#define FALSE               0
#endif

#ifndef TRUE
#define TRUE                1
#endif

#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

///////////////////////////////////////////////////////////////
// C functions for external declarations that call the appropriate C++ methods
#define EXPORT
#define IMPORT

///////////////////////////////////////////////////////////////
#ifndef EXTERN_C
#ifdef __cplusplus
#define EXTERN_C    extern "C"
#else
#define EXTERN_C    extern
#endif
#endif

//DLL_EXPORT
// Used for dll exporting and importing
#define  DLL_EXPORT   extern "C" EXPORT
#define  DLL_IMPORT   extern "C" IMPORT
#ifdef __cplusplus
// Can't use extern "C" when DLL exporting a class
#define  DLL_CLASS_EXPORT   DLL_EXPORT
#define  DLL_CLASS_IMPORT   IMPORT
// Can't use extern "C" when DLL exporting a global
#define  DLL_GLOBAL_EXPORT   extern DLL_EXPORT
#define  DLL_GLOBAL_IMPORT   extern IMPORT
#endif

#if defined _WIN32 || defined __CYGWIN__
#ifndef OFC_API
#ifdef __GNUC__
#ifdef OFC_EXPORT
#define OFC_API __attribute__ ((dllexport))

#else
#define OFC_API __attribute__ ((dllimport))
#endif
#else
#ifdef OFC_EXPORT
#define OFC_API __declspec(dllexport)
#else
#define OFC_API __declspec(dllimport) // Note: actually gcc seems to also supports this syntax.
#endif
#endif
#endif
#define OFC_LOCAL_API
#else
#ifndef OFC_API
#if defined(OFC_EXPORT) && (__GNUC__ >= 4)
#define OFC_API __attribute__ ((visibility ("default")))
#define OFC_LOCAL_API  __attribute__ ((visibility ("hidden")))
#else
#define OFC_API
#define OFC_LOCAL_API
#endif
#endif
#endif
//////////////////////////////////////////////////////////////////////////
#ifndef SAFE_DELETE
#define SAFE_DELETE(p)       { if(p) { delete (p); (p)=NULL; } }
#endif
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)    { if(p) { (p)->Release(); (p)=NULL; } }
#endif
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p); (p)=NULL; } }
#endif

//////////////////////////////////////////////////////////////////////////
#define TOUPPER(c) ((((c) >= 'a') && ((c) <= 'z')) ? (c)+'A'-'a' : (c))
#define TONIBBLE(c) ((((c) >= 'A')&&((c) <= 'F')) ? (((c)-'A')+10) : ((c)-'0'))
#define BYTES_TO_KBPS(n) (float)(((((float)n)*8.0f)/1024.0f))

//////////////////////////////////////////////////////////////////////////
#define isSJIS(a,b) ((a >= 0x81 && a <= 0x9f || a >= 0xe0 && a<=0xfc) && (b >= 0x40 && b <= 0x7e || b >= 0x80 && b<=0xfc))
#define isEUC(a) (a >= 0xa1 && a <= 0xfe)
#define isASCII(a) (a <= 0x7f)
#define isPLAINASCII(a) (((a >= '0') && (a <= '9')) || ((a >= 'a') && (a <= 'z')) || ((a >= 'A') && (a <= 'Z')))
#define isUTF8(a,b) ((a & 0xc0) == 0xc0 && (b & 0x80) == 0x80 )
#define isESCAPE(a,b) ((a == '&') && (b == '#'))
#define isHTMLSPECIAL(a) ((a == '&') || (a == '\"') || (a == '\'') || (a == '<') || (a == '>'))

//////////////////////////////////////////////////////////////////////////
//#define SWAP16(v) (((v&0xff)<<8) | ((v&0xff00)>>8))
//#define SWAP24(v) (((v&0xff)<<16) | ((v&0xff00)) | ((v&0xff0000)>>16))
//#define SWAP32(v) (((v&0xff)<<24) | ((v&0xff00)<<8) | ((v&0xff0000)>>8) | ((v&0xff000000)>>24))
//#define SWAP64(v) ((SWAP4(v)|((uint64_t)SWAP4(v+4)<< 32)))
//
//#if _BYTE_ORDER == _BIG_ENDIAN
//#define CHECK_ENDIAN16(v) v=SWAP16(v)
//#define CHECK_ENDIAN24(v) v=SWAP24(v)
//#define CHECK_ENDIAN32(v) v=SWAP32(v)
//#define CHECK_ENDIAN64(v) v=SWAP64(v)
//#else//!_BIG_ENDIAN
//#define CHECK_ENDIAN16(v)
//#define CHECK_ENDIAN24(v)
//#define CHECK_ENDIAN32(v)
//#define CHECK_ENDIAN64(v)
//#endif //_BIG_ENDIAN

#if defined(__ANDROID__)
#include <sys/endian.h>
#elif defined(__linux__)
#include <endian.h>
#elif defined(_MAC) || defined(__APPLE__)
#include <sys/_endian.h>
#endif

#ifndef __swap16gen
#define __swap16(x)                                     \
		(uint16)((((uint16)(x) & 0xff) << 8) |          \
			((uint16)(x) & 0xff00) >> 8)
#endif //__swap16gen

#ifndef __swap32gen
#define __swap32gen(x)                                     \
		(uint32)((((uint32)(x) & 0xff) << 24) |			\
			((uint32)(x) & 0xff00) << 8 |               \
			((uint32)(x) & 0xff0000) >> 8 |	            \
			((uint32)(x) & 0xff000000) >> 24)
#endif //__swap32gen

#ifndef __swap64gen
#define __swap64gen(x)							\
	(uint64)((((uint64)(x) & 0xff) << 56) |			\
	    ((uint64)(x) & 0xff00ULL) << 40 |			\
	    ((uint64)(x) & 0xff0000ULL) << 24 |			\
	    ((uint64)(x) & 0xff000000ULL) << 8 |			\
	    ((uint64)(x) & 0xff00000000ULL) >> 8 |			\
	    ((uint64)(x) & 0xff0000000000ULL) >> 24 |		\
	    ((uint64)(x) & 0xff000000000000ULL) >> 40 |		\
	    ((uint64)(x) & 0xff00000000000000ULL) >> 56)
#endif //__swap64gen

#ifndef __swap64
#define __swap64 __swap64gen
#endif
#ifndef __swap32
#define __swap32 __swap32gen
#endif
#ifndef __swap16
#define __swap16 __swap16gen
#endif

//在APPLE手机的__IPHONE_8_0以前的版本是没有ntohq宏的，因此在此需要重新定义
#ifndef ntohq
#define ntohq(x) __swap64(x)
#define htonq(x) __swap64(x)
#endif

#define ofc_ntohs(x) __swap16(x)
#define ofc_htons(x) __swap16(x)
#define ofc_ntohl(x) __swap32(x)
#define ofc_htonl(x) __swap32(x)
#define ofc_ntohll(x) ntohq(x)
#define ofc_htonll(x) htonq(x)

#if !defined(MAKE_WORD)
#define MAKE_WORD(high, low) \
    ((WORD)(((BYTE)(low)) | ((WORD)((BYTE)(high))) << 8))
#endif

#if !defined(GET_LOW)
#define GET_LOW(word)   ((BYTE)word)
#endif

#if !defined(GET_HIGH)
#define GET_HIGH(word)   ((((WORD)word) >> 8) & 0x00FF)
#endif

#if !defined(MAKE_DWORD)
#define MAKE_DWORD(high, low) \
	((DWORD)(((WORD)(low)) | ((DWORD)((WORD)(high))) << 16))
#endif

#if !defined(GET_DLOW)
#define GET_DLOW(dword)   ((WORD)dword)
#endif

#if !defined(GET_DHIGH)
#define GET_DHIGH(dword)   ((((DWORD)dword) >> 16) & 0x0000FFFF)
#endif

#if !defined(MAKE_ULLONG)
#define MAKE_ULLONG(high, low) \
	((UINT64)(((DWORD)(low)) | ((UINT64)((DWORD)(high))) << 32))
#endif

#if !defined(GET_ULLOW)
#define GET_ULLOW(ull)   ((DWORD)ull)
#endif

#if !defined(GET_ULHIGH)
#define GET_ULHIGH(ull)   ((((UINT64)ull) >> 32) & 0x00000000FFFFFFFFULL)
#endif

// determine number of elements in an array (not bytes)
#ifndef _countof
#define _countof(array) (sizeof(array)/sizeof(array[0]))
#endif

//disable use copy construct and '=' operator macro
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
    private: \
        TypeName(const TypeName&); \
        void operator=(const TypeName&)

#endif //__FCDEFS_H_
