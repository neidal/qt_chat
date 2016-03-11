/*
* 数据类型
*/

#ifndef __DATA_TYPE_H_
#define __DATA_TYPE_H_

#pragma once
#include <qglobal.h>
typedef qint16 int16;
typedef quint16 uint16;
typedef qint32 int32;
typedef quint32 uint32;
typedef qint64 int64;
typedef quint64 uint64;

#ifdef Q_OS_WIN
typedef float float32;
typedef double float64;
#else
typedef float float32;
typedef double float64;
#endif

// 
// 
// //数据类型
// typedef signed char int8;
// typedef int8 int8_t;
// typedef unsigned char uint8;
// typedef uint8 u_int8_t;
// typedef uint8 uint8_t;
// typedef unsigned char		UInt8;
// 
// typedef signed short int16;
// typedef unsigned short uint16;
// typedef int16 int16_t;
// typedef uint16 u_int16_t;
// typedef uint16 uint16_t;
// 
// typedef signed int int32;
// typedef int32 int32_t;
// typedef uint32 u_int32_t;
// typedef uint32 uint32_t;
// typedef signed int SInt32;
// typedef unsigned int		UInt32;
// 
// typedef signed __int64 int64;
// typedef unsigned __int64 uint64;
// typedef int64 int64_t;
// typedef uint64 u_int64_t;
// typedef uint64 uint64_t;
// 
// 
// typedef unsigned char byte;
// typedef unsigned long       ulong;
// 
// #if defined(_WIN64)
// typedef uint64 uintptr;
// typedef uint32 EventValue;
// typedef uint32 MessageValue;
// typedef uint32 ServiceValue;
// typedef uint32 uin;
// typedef uint32 tid;
// typedef uint64 param;
// #else
// typedef uint32 uintptr;
// typedef uint32 EventValue;
// typedef uint32 MessageValue;
// typedef uint32 ServiceValue;
// typedef uint32 uin;
// typedef uint32 tid;
// typedef uint32 param;
// #endif
// 
// typedef uint32 Cookie;
// 
// #ifndef TRUE
// #define TRUE (1)
// #endif
// 
// #ifndef FALSE
// #define FALSE (0)
// #endif
// 
// #ifndef BOOL
// #define BOOL int
// #endif
// 
// #include <string>
// #include <map>
// using namespace std;
// 
// #ifdef UNICODE
// #define tstring std::wstring
// #else
// #define tstring std::string
// #endif
// 
 #endif //__DATA_TYPE_H_
// 
