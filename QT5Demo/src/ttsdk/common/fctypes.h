#ifndef __FCTYPES_H_
#define __FCTYPES_H_

#ifdef _WIN32
#if defined(__ARM_EABI__) || defined(__ANDROID__) || defined(__linux__)
#undef _WIN32
#endif
#endif

#ifdef _WIN32
#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 从 Windows 头中排除极少使用的资料
#endif
#define WIN32_LEAN_AND_MEAN		// 从 Windows 头中排除winsock.h

// 如果必须要针对低于以下指定版本的平台，请修改下列定义。
// 有关不同平台对应值的最新信息，请参考 MSDN。
#ifndef WINVER                          // 指定要求的最低平台是 Windows XP。
#define WINVER 0x0501           // 将此值更改为相应的值，以适用于 Windows 的其他版本。
#endif

#ifndef _WIN32_WINNT            // 指定要求的最低平台是 Windows XP。
#define _WIN32_WINNT 0x0501     // 将此值更改为相应的值，以适用于 Windows 的其他版本。
#endif

#ifndef _WIN32_WINDOWS          // 指定要求的最低平台是 Windows 98。
#define _WIN32_WINDOWS 0x0410 // 将此值更改为适当的值，以适用于 Windows Me 或更高版本。
#endif

#ifndef _WIN32_IE                       // 指定要求的最低平台是 Internet Explorer 7.0。
#define _WIN32_IE 0x0700        // 将此值更改为相应的值，以适用于 IE 的其他版本。
#endif

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include <sys/types.h>

#ifndef _x64_
#if defined(__x86_64__) || (__WORDSIZE == 64) 
#define _x64_
#endif
#endif

#ifndef NDEBUG
#ifndef DEBUG
#define DEBUG
#endif
#endif

#ifdef DEBUG
#ifndef _DEBUG
#define _DEBUG
#endif
#endif // DEBUG

#ifdef UNICODE
#ifndef _UNICODE
#define _UNICODE
#endif
#endif // UNICODE

#ifndef _WIN32
#define cdecl
#ifndef CDECL
#define CDECL cdecl
#else
#define FC_NO_VTABLE _declspec(novtable)
#endif
#ifdef _MAC
#define CALLBACK    PASCAL
#ifdef _68K_
#define PASCAL      __pascal
#else
#define PASCAL
#endif
#else
#define CALLBACK    __stdcall
#define PASCAL      __stdcall
#endif
#endif

#ifndef FCAPI
#if defined (__ANDROID__) || defined (__APPLE__)
#define FCAPI
#else
#define FCAPI		__stdcall
#endif
#endif

typedef unsigned long ULONG;
//#ifdef __GLIBC_HAVE_LONG_LONG
#ifndef _WIN32
typedef long long __int64;
#endif
typedef unsigned long long __uint64;
//#elif !defined(_x64_)
//typedef signed long long __int64;
//typedef unsigned long long __uint64;
//#endif // __GLIBC_HAVE_LONG_LONG
#ifdef _x64_
typedef __uint64 ULLONG_PTR, *PULLONG_PTR;
#else
typedef unsigned long long ULLONG_PTR, *PULLONG_PTR;
#endif

#ifdef __NO_COMPATIBLE_WIN32
typedef signed char         int8
typedef unsigned char       uint8
typedef signed short        int16
typedef unsigned short      uint16
typedef signed int          int32
typedef unsigned int        uint32
typedef __int64             int64
typedef __uint64            uint64
typedef float               float32
typedef double              float64

// UNICODE (Wide Character) types
typedef wchar_t             wchar;    // wc,   16-bit UNICODE character
typedef unsigned wchar_t    wbyte;    // wb,   16-bit UNICODE character

// Neutral ANSI/UNICODE types and macros
#ifdef  _UNICODE
typedef wchar               tchar;
typedef wbyte               tbyte;
#define __TEXT(quote)       L##quote
#else   //_UNICODE
typedef char                tchar;
typedef unsigned char       tbyte;
#define __TEXT(quote)       quote
#endif //!_UNICODE
#else  //__NO_COMPATIBLE_WIN32
#if !defined(_BASETSD_H_)
typedef signed char         INT8, *PINT8;
typedef signed short        INT16, *PINT16;
typedef signed int          INT32, *PINT32;
typedef __int64             INT64, *PINT64;
typedef unsigned char       UINT8, *PUINT8;
typedef unsigned short      UINT16, *PUINT16;
typedef unsigned int        UINT32, *PUINT32;
typedef __uint64            UINT64, *PUINT64;

// The INT_PTR is guaranteed to be the same size as a pointer.  Its
// size with change with pointer size (32/64).  It should be used
// anywhere that a pointer is cast to an integer type. UINT_PTR is
// the unsigned variation.

#ifdef _x64_
    typedef __int64 INT_PTR, *PINT_PTR;
    typedef __uint64 UINT_PTR, *PUINT_PTR;

    typedef __int64 LONG_PTR, *PLONG_PTR;
    typedef __uint64 ULONG_PTR, *PULONG_PTR;
    #define __int3264   __int64

#else
    typedef int INT_PTR, *PINT_PTR;
    typedef unsigned int UINT_PTR, *PUINT_PTR;

    typedef long LONG_PTR, *PLONG_PTR;
    typedef unsigned long ULONG_PTR, *PULONG_PTR;
    #define __int3264   __int32
#endif
#endif //!_BASETSD_H_

#ifndef _WIN32
typedef short               SHORT;
typedef long                LONG;
typedef int                 INT;
typedef unsigned int        UINT, DWORD;

#ifndef __APPLE__
typedef long                BOOL;
#else
#include <objc/objc.h>
#endif

typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef float               FLOAT;
typedef double              DOUBLE;
typedef BOOL                *PBOOL;
typedef UINT                *PUINT;
typedef DWORD               *PDWORD;

typedef BYTE                *PBYTE;
typedef INT                 *PINT;
typedef LONG                *PLONG;
typedef ULONG               *PULONG;
typedef SHORT               *PSHORT;
typedef WORD                *PWORD;
typedef FLOAT               *PFLOAT;
typedef DOUBLE              *PDOUBLE;

#ifndef CONST
#define CONST               const
#endif

#ifndef VOID
#define VOID                void
#endif

typedef void                *PVOID;
typedef const void          *PCVOID;

// UNICODE (Wide Character) types
typedef wchar_t WCHAR;    // wc,   16-bit UNICODE character
typedef WCHAR * PWCHAR, * PWSTR;
typedef CONST WCHAR * PCWSTR;

// ANSI (Multi-byte Character) types
typedef char CHAR;
typedef CHAR * PCHAR, * PSTR;
typedef CONST CHAR * PCSTR;

// Neutral ANSI/UNICODE types and macros
#ifdef  _UNICODE

#ifndef _TCHAR_DEFINED
typedef WCHAR TCHAR, *PTCHAR;
typedef WCHAR TBYTE, *PTBYTE;
#define _TCHAR_DEFINED
#endif /* !_TCHAR_DEFINED */

typedef PWSTR PTSTR;
typedef PCWSTR PCTSTR;
#define __TEXT(quote) L##quote

#else   //_UNICODE

#ifndef _TCHAR_DEFINED
typedef char TCHAR, *PTCHAR;
typedef unsigned char TBYTE, *PTBYTE;
#define _TCHAR_DEFINED
#endif /* !_TCHAR_DEFINED */

typedef PSTR PTSTR;
typedef PCSTR PCTSTR;
#define __TEXT(quote) quote

#endif //!_UNICODE

typedef PVOID HANDLE;
typedef HANDLE HINSTANCE, *PHANDLE;
typedef HINSTANCE HMODULE;
#endif // !_WIN32

#define int8                INT8
#define uint8               UINT8
#define int16               INT16
#define uint16              UINT16
#define int32               INT32
#define uint32              UINT32
#define int64               INT64
#define uint64              UINT64
#define FLOAT32             FLOAT
#define FLOAT64             DOUBLE
#define float32             FLOAT
#define float64             DOUBLE

#endif //!__NO_COMPATIBLE_WIN32

typedef struct _int128
{
  int64 high;
  int64 low;
}int128, *pint128;

typedef struct _uint128
{
  uint64 high;
  uint64 low;
}uint128, *puint128;

#endif // __FCTYPES_H_
