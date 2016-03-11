#ifndef _FCSTRING_H_
#define _FCSTRING_H_
#include "fctypes.h"
#ifdef _USE_STD_STRING
#include <string>
#else
#include <ctype.h>
#include <wchar.h>
#endif
#include <string.h>
#ifdef _USE_STD_IOSTREAM
#include <iostream>
#include <fstream>
#endif
using namespace std;

#ifndef _WIN32
#ifndef ZeroMemory
#define ZeroMemory(p, t)     memset(p,0,t)
#endif  //ZeroMemory

///////////////////////////////////////
//区分是否使用Unicode方式
///////////////////////////////////////
#ifdef _UNICODE

#define tstr wstr
#ifdef _USE_STD_IOSTRAAM
#define tfstream wfstream
#endif
#else //_UNICODE

#define tstr str
#ifdef _USE_STD_IOSTRAAM
#define tfstream fstream
#endif

#endif //!_UNICODE

#ifdef _USE_ININE_WSTRING
#ifdef __cplusplus
extern "C"{
#endif

inline char * _strlwr(char *s)
{
	if (!s) return NULL;
	while (*s)
	{
		*s = (char)tolower(*s);
		++s;
	}
	return s;
}

inline wchar_t * _wcsrchr(
	const wchar_t * str,
	wchar_t ch
	)
{
	wchar_t *start = (wchar_t *)str;

	while (*str++)                       /* find end of str */
		;
	/* search towards front */
	while (--str != start && *str != (wchar_t)ch)
		;

	if (*str == (wchar_t)ch)             /* wchar_t found ? */
		return((wchar_t *)str);

	return(NULL);
}

//////////////////////////////////////////////////////////////////////////
// 下面这些函数是从vc copy来的。linux也使用这些函数。但是不支持big_endor
inline size_t _wcslen(const wchar_t * wcs)
{
	const wchar_t *eos = wcs;
	while (*eos++);
	return((size_t)(eos - wcs - 1));
}


inline size_t _wcsrtombs(char *_Dest, const wchar_t *_PSource, size_t _Count)
{
	if (!_Dest || !_PSource || _Count <= 0)
	{
		return 0;
	}

	size_t  count;
	size_t srclen;
	const wchar_t *src = _PSource;

	srclen = _wcslen(_PSource);
	if (_Count > srclen)
	{
		_Count = srclen;
	}
	// 下面的*src++ 让人费解, 只需要src++就够了,BTW:用前置++效率更高
	for (count = 0; count < _Count; count++, src++)
	{
		if (*src == '\0')
			break;

		_Dest[count] = (char)*src;
	}

	_Dest[count] = '\0';
	return count;
}

inline wchar_t * _wcsncpy(
	wchar_t * dest,
	const wchar_t * source,
	size_t count
	)
{
	wchar_t *start = dest;

	while (count && (*dest++ = *source++))    /* copy str */
		count--;

	if (count)                              /* pad out with zeroes */
		while (--count)
			*dest++ = L'\0';

	return(start);
}

inline int _wcscmp(
	const wchar_t * src,
	const wchar_t * dst
	)
{
	int ret = 0;

	while (((ret = (int)(*src - *dst)) == 0) && *dst)
	{
		++src;
		++dst;
	}

	if (ret < 0)
		ret = -1;
	else if (ret > 0)
		ret = 1;

	return(ret);
}

inline int _wcsicmp(
	const wchar_t * src,
	const wchar_t * dst
	)
{
	int ret = 0;
	wchar_t lCmpA = *src;
	wchar_t lCmpB = *dst;

	if (lCmpA >= 0x41 && lCmpA <= 0x5a)
		lCmpA += 0x20;

	if (lCmpB >= 0x41 && lCmpB <= 0x5a)
		lCmpB += 0x20;

	while (((ret = (int)(lCmpB - lCmpA)) == 0) && lCmpB)
	{
		++src;
		++dst;
		lCmpA = *src;
		lCmpB = *dst;

		if (lCmpA >= 0x41 && lCmpA <= 0x5a)
			lCmpA += 0x20;

		if (lCmpB >= 0x41 && lCmpB <= 0x5a)
			lCmpB += 0x20;

	}

	if (ret < 0)
		ret = -1;
	else if (ret > 0)
		ret = 1;

	return(ret);
}

inline wchar_t * _wcsncat(
	wchar_t * front,
	const wchar_t * back,
	size_t count
	)
{
	wchar_t *start = front;

	while (*front++)
		;
	front--;

	while (count--)
		if (!(*front++ = *back++))
			return(start);

	*front = L'\0';
	return(start);
}


inline int _wcsncmp(
	const wchar_t * first,
	const wchar_t * last,
	size_t count
	)
{
	if (!count)
		return(0);

	while (--count && *first && *first == *last)
	{
		first++;
		last++;
	}

	return((int)(*first - *last));
}

inline wchar_t * _wcscpy(wchar_t * dst, const wchar_t * src)
{
	wchar_t * cp = dst;

	while ((*cp++ = *src++))
		;               /* Copy src over dst */

	return(dst);
}
inline wchar_t * _wcscat(
	wchar_t * dst,
	const wchar_t * src
	)
{
	wchar_t * cp = dst;

	while (*cp)
		cp++;                   /* find end of dst */

	while ((*cp++ = *src++));       /* Copy src to end of dst */

	return(dst);                  /* return dst */

}

inline wchar_t * _wcschr(
	const wchar_t * str,
	wchar_t ch
	)
{
	while (*str && *str != (wchar_t)ch)
		str++;

	if (*str == (wchar_t)ch)
		return((wchar_t *)str);
	return(NULL);
}

inline wchar_t * _wcsstr(
	const wchar_t * wcs1,
	const wchar_t * wcs2
	)
{
	wchar_t *cp = (wchar_t *)wcs1;
	wchar_t *s1, *s2;

	if (!*wcs2)
		return (wchar_t *)wcs1;

	while (*cp)
	{
		s1 = cp;
		s2 = (wchar_t *)wcs2;

		while (*s1 && *s2 && !(*s1 - *s2))
			s1++, s2++;

		if (!*s2)
			return(cp);

		cp++;
	}

	return(NULL);
}

inline wchar_t * _wcslwr(wchar_t *str)
{
	int lilen = (int)_wcslen(str);
	for (int i = 0; i < lilen; ++i)
	{
		str[i] = towlower(str[i]);
	}
	return str;
}

#ifdef __cplusplus
}
#endif
#endif //_USE_ININE_WSTRING

////////////////////////////////////////////
//  以为为字符串操作定义
////////////////////////////////////////////
#ifdef _UNICODE

#ifndef _T
#define _T(x)   __TEXT(x)
#endif

/* Formatted i/o */

#define _tprintf        wprintf
#define _ftprintf       fwprintf
#define _vtprintf       vwprintf
#define _sntprintf      swprintf
#define _vsnprintf      vswprintf
#define _tscanf         wscanf

/* Unformatted i/o */

#define _fgettc         fgetwc
#define _fgetts         fgetws
#define _fputtc         fputwc
#define _fputts         fputws
#define _gettc          getwc
#define _getts          _getws
#define _puttc          putwc
#define _putts          _putws

/* str conversion functions */
#define _wtoi(wstr) wcstol(wstr,NULL,10)
#define _tcstoul    wcstoul
#define _tstol      _wtol
#define _tstoi      _wtoi
#define _tstoi64    _wtoi64

#define _itot       _itow
#define _ltot       _ltow
#define _ultot      _ultow
#define _i64tot     _i64tow
#define _ui64tot    _ui64tow

/* str functions */

#define _tcscat         _wcscat
#define _tcschr         _wcschr
#define _tcscpy         _wcscpy
#define _tcscspn        wcscspn // 返回不包含第二个字符串的的初始数目
#define _tcslen         _wcslen
#define _tcsnlen        _wcsnlen
#define _tcsncat        _wcsncat
#define _tcsncpy        _wcsncpy
#define _tcspbrk        wcspbrk
#define _tcsrchr        _wcsrchr
#define _tcsspn         wcsspn // 返回包含第二个字符串的初始数目
#define _tcsstr         _wcsstr
#define _tcstok         wcstok
#define _tcserror       _wcserror

#define _tcsdup         _wcsdup
#define _tcsnset        _wcsnset
#define _tcsrev         _wcsrev
#define _tcsset         _wcsset

#define _tcscmp         _wcscmp
#define _tcsicmp        _wcsicmp
#define _tcsnccmp       _wcsncmp
#define _tcsncmp        _wcsncmp
#define _tcsncicmp      _wcsnicmp
#define _tcsnicmp       _wcsnicmp

#define _tcscoll        wcscoll
#define _tcsicoll       _wcsicoll
#define _tcsnccoll      _wcsncoll
#define _tcsncoll       _wcsncoll
#define _tcsncicoll     _wcsnicoll
#define _tcsnicoll      _wcsnicoll


/* Time functions */

#define _tasctime   _wasctime
#define _tcsftime   wcsftime

/* Stdio functions */

#define _tfopen     _wfopen
#define _tfopen_s   _wfopen_s

#ifdef _USE_STD_IOSTRAAM
/* Io functions */
#define _tstreambuf  wstreambuf
#define _tistream    wistream
#define _tostream    wostream
#define _tiostream   wiostream


#define _tfilebuf    wfilebuf
#define _tifstream   wifstream
#define _tofstream   wofstream
#define _tfstream    wfstream
#endif

#define _Zerostr(str,len) \
  memset(str,0,(len)*sizeof(wchar_t))

#else //UNICODE

#ifndef _T
#define _T(x)   x
#endif

/* Formatted i/o */

#define _tprintf        printf
#define _ftprintf       fprintf
#define _vtprintf       vprintf
#define _sntprintf      snprintf
#define _vsnprintf      vsnprintf
#define _tscanf         scanf

/* Unformatted i/o */

#define _fgettc         fgetc
#define _fgetts         fgets
#define _fputtc         fputc
#define _fputts         fputs
#define _gettc          getc
#define _getts          _gets
#define _puttc          putc
#define _putts          _puts

/* str conversion functions */
#define _tcstoul    strtoul
#define _tstol      atol
#define _tstoi      atoi
#define _tstoi64    _atoi64

#define _itot       _itoa
#define _ltot       _ltoa
#define _ultot      _ultoa
#define _i64tot     _i64toa
#define _ui64tot    _ui64toa

/* str functions */

#define _tcscat         strcat
#define _tcschr         strchr
#define _tcscpy         strcpy
#define _tcscspn        strcspn // 返回不包含第二个字符串的的初始数目
#define _tcslen         strlen
#define _tcsnlen        strnlen
#define _tcsncat        strncat
#define _tcsncpy        strncpy
#define _tcspbrk        strpbrk
#define _tcsrchr        strrchr
#define _tcsspn         strspn // 返回包含第二个字符串的初始数目
#define _tcsstr         strstr
#define _tcstok         strtok
#define _tcserror       _strerror

#define _tcsdup         _strdup
#define _tcsnset        _strnset
#define _tcsrev         _strrev
#define _tcsset         _strset

#define _tcscmp         strcmp
#define _tcsicmp        _stricmp
#define _tcsnccmp       strncmp
#define _tcsncmp        strncmp
#define _tcsncicmp      _strnicmp
#define _tcsnicmp       _strnicmp

#define _tcscoll        strcoll
#define _tcsicoll       _stricoll
#define _tcsnccoll      _strncoll
#define _tcsncoll       _strncoll
#define _tcsncicoll     _strnicoll
#define _tcsnicoll      _strnicoll


/* Time functions */

#define _tasctime   asctime
#define _tcsftime   strftime

/* Stdio functions */

#define _tfopen     _fopen
#define _tfopen_s   _fopen_s

#ifdef _USE_STD_IOSTREAM
/* Io functions */
#define _tstreambuf  streambuf
#define _tistream    istream
#define _tostream    ostream
#define _tiostream   iostream


#define _tfilebuf    filebuf
#define _tifstream   ifstream
#define _tofstream   ofstream
#define _tfstream    fstream
#endif

#define _ZeroString(str,len)  memset(str,0,len)
#endif //!_UNICODE

#define atoul(str)		strtoul(str, NULL, 10)
#define atoull(str)		strtoull(str, NULL, 10)
#endif //!_WIN32

#ifdef _WIN32
#define atoul(str)		strtoul(str, NULL, 10)
#if defined(_MSC_VER) && _MSC_VER == 1500
#define atoull(str)		_atoi64(str)
#else
#define atoull(str)		strtoull(str, NULL, 10)
#endif
#endif //!_WIN32

#endif // _FCSTRING_H_
