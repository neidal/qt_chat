//
// SndaBase.h
#pragma once

#include "windows.h"
#include "string.h"
#include "assert.h"
#include "../global/types.h"

#if defined(_WIN32) || defined(_WIN64)
#define SNDACALL			__stdcall
#else
#define SNDACALL
#endif

#if !defined(interface)
#define interface	struct
#endif

#if !defined(PURE)
#define PURE		= 0
#endif

#define SNDACALLBACK		SNDACALL
#define SNDAMETHOD(Type)	virtual Type SNDACALL
#define SNDAAPI(Type)		extern "C" Type SNDACALL

#ifdef _USE_LINUX
#define SNDADLLAPI 
#else
#define SNDADLLAPI extern "C" _declspec(dllexport)
#endif

#ifdef _USE_LINUX
#define CLASSEXPORT 
#else
#define CLASSEXPORT _declspec(dllexport)
#endif

#ifndef PARRAYSIZE
#define PARRAYSIZE(array) ((sizeof(array)/sizeof(array[0])))
#endif

#ifndef DELETEP
#define DELETEP(ptr) \
	if(NULL != (ptr)) \
{ \
	delete (ptr); \
	(ptr) = NULL; \
}
#endif

#ifndef STRNCPY
#define STRNCPY(dstPtr,srcPtr) \
memset(dstPtr, 0x0, sizeof(dstPtr)/sizeof(wchar_t)); \
wcsncpy(dstPtr, srcPtr, sizeof(dstPtr)/sizeof(wchar_t)); 
#endif

#ifndef ZEROMEM
#define ZEROMEM(ptr) \
memset(ptr, 0x0,sizeof(ptr));
#endif


#ifndef NULL
#define NULL 0
#endif


#ifndef NULL
#define NULL 0
#endif

#ifndef IN
#define IN	
#endif

#ifndef OUT
#define OUT
#endif

#ifndef ASSERT
#define ASSERT assert
#endif
