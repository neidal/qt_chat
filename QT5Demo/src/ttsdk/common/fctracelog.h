#ifndef __FCTARCELOG_H_
#define __FCTARCELOG_H_

#include "fcguid.h"
#include "fcstring.h"
#include "fcsocket.h"
#include "fctrace.h"

#ifndef SINGLELOG_BUFFER_SIZE
#define SINGLELOG_BUFFER_SIZE     1024      // 一条日志1K大小
#endif

#ifdef _WIN32
#ifndef snprintf
#define snprintf _snprintf
#endif
#endif

//分隔字符
#define LOG_SYMBOL endl
#define LOG_BEGIN  1119

#pragma pack(push, 1)
struct TRACE_LOG_BUFFER
{
	int log_level;
	int log_type;
    int log_head_size;
	int log_size;
	char log_buff[SINGLELOG_BUFFER_SIZE];

	TRACE_LOG_BUFFER()
	{
	    Init();
	}

    void Init()
    {
		log_level = LOG_LEVEL_NOTICE;
		log_type = DEF_LOG_DISPLAYTYPE;
        log_head_size = 0;
		log_size = 0;
		log_buff[0] = 0;
    }

	inline TRACE_LOG_BUFFER & operator <<(void * _value);
    inline TRACE_LOG_BUFFER & operator <<(char * _value);
    inline TRACE_LOG_BUFFER & operator <<(const char * _value);
    inline TRACE_LOG_BUFFER & operator <<(wchar_t * _value);
    inline TRACE_LOG_BUFFER & operator <<(const wchar_t * _value);
    inline TRACE_LOG_BUFFER & operator <<(bool _value);
    inline TRACE_LOG_BUFFER & operator <<(char);
    inline TRACE_LOG_BUFFER & operator <<(short);
    inline TRACE_LOG_BUFFER & operator <<(int);
    inline TRACE_LOG_BUFFER & operator <<(long _value);
    inline TRACE_LOG_BUFFER & operator <<(long long _value);
    inline TRACE_LOG_BUFFER & operator <<(unsigned char);
    inline TRACE_LOG_BUFFER & operator <<(unsigned short);
    inline TRACE_LOG_BUFFER & operator <<(unsigned int);
    inline TRACE_LOG_BUFFER & operator <<(unsigned long _value);
    inline TRACE_LOG_BUFFER & operator <<(unsigned long long _value);
    inline TRACE_LOG_BUFFER & operator <<(float _value);
    inline TRACE_LOG_BUFFER & operator <<(double _value);
#ifdef _USE_STD_STRING
    inline TRACE_LOG_BUFFER & operator <<(string & _value);
    inline TRACE_LOG_BUFFER & operator <<(const string  & _value);
    inline TRACE_LOG_BUFFER & operator <<(wstring  & _value);
    inline TRACE_LOG_BUFFER & operator <<(const wstring  & _value);
#endif
    inline TRACE_LOG_BUFFER & operator <<(const IN_ADDRINFO& _value);
    inline TRACE_LOG_BUFFER & operator <<(const GUID& _value);

private:
	TRACE_LOG_BUFFER& OperateString(const char *_value, int _len)
	{
		if (!_value || log_size >= SINGLELOG_BUFFER_SIZE)
			return *this;

		if ((_len + log_size) > SINGLELOG_BUFFER_SIZE)
			return *this;

		memcpy(log_buff+log_size, _value, _len);
		log_size += _len;
		log_buff[log_size] = '\0';
		return *this;
	}

	TRACE_LOG_BUFFER& OperateString(const char *_value)
	{
		if (!_value || log_size >= SINGLELOG_BUFFER_SIZE)
			return *this;

		int len = (int)strlen(_value);
		if ((len + log_size) > SINGLELOG_BUFFER_SIZE)
			return *this;

		memcpy(log_buff+log_size, _value, len);
		log_size += len;
		log_buff[log_size] = '\0';
		return *this;
	}

	TRACE_LOG_BUFFER& OperateWString(const wchar_t *_value, int _len)
	{
		if (!_value || log_size >= SINGLELOG_BUFFER_SIZE)
			return *this;

		int len = (int)(_len*sizeof(wchar_t));
		if ((len + log_size) > SINGLELOG_BUFFER_SIZE)
			return *this;

		memcpy(log_buff+log_size, _value, len);
		log_size += len;
		log_buff[log_size] = '\0';
		return *this;
	}

	TRACE_LOG_BUFFER& OperateWString(const wchar_t *_value)
	{
		if (!_value || log_size >= SINGLELOG_BUFFER_SIZE)
			return *this;

		int len = (int)(wcslen(_value)*sizeof(wchar_t));
		if ((len + log_size) > SINGLELOG_BUFFER_SIZE)
			return *this;

		memcpy(log_buff+log_size, _value, len);
		log_size += len;
		log_buff[log_size] = '\0';
		return *this;
	}
};

typedef TRACE_LOG_BUFFER * PTRACE_LOG_BUFFER;
#pragma pack(pop)

inline TRACE_LOG_BUFFER& TRACE_LOG_BUFFER::operator<<(void* _value)
{
    char buff[32];
	int len = snprintf(buff, 32, "%p", _value);
	return OperateString(buff, len);
}

inline TRACE_LOG_BUFFER& TRACE_LOG_BUFFER::operator <<(const char * _value)
{
	return OperateString(_value);
}

inline TRACE_LOG_BUFFER& TRACE_LOG_BUFFER::operator <<(char * _value)
{
    return operator << ((const char*)_value);
}

inline TRACE_LOG_BUFFER& TRACE_LOG_BUFFER::operator <<(const wchar_t * _value)
{
	return OperateWString(_value);
}

inline TRACE_LOG_BUFFER& TRACE_LOG_BUFFER::operator <<(wchar_t * _value)
{
    return operator << ((const wchar_t*)_value);
}

inline TRACE_LOG_BUFFER& TRACE_LOG_BUFFER::operator <<(bool _value)
{
	if (false == _value)
	{
		*this <<"false";
	}
	else
	{
		*this <<"true";
	}

	return *this;
}

inline TRACE_LOG_BUFFER& TRACE_LOG_BUFFER::operator <<(long _value)
{
    char buff[32];
	int len = snprintf(buff, 32, "%ld", _value);
	return OperateString(buff, len);
}

inline TRACE_LOG_BUFFER& TRACE_LOG_BUFFER::operator <<(char _value)
{
    return operator << ((long)_value);
}

inline TRACE_LOG_BUFFER& TRACE_LOG_BUFFER::operator <<(short _value)
{
    return operator << ((long)_value);
}

inline TRACE_LOG_BUFFER& TRACE_LOG_BUFFER::operator <<(int _value)
{
    return operator << ((long)_value);
}

inline TRACE_LOG_BUFFER& TRACE_LOG_BUFFER::operator <<(unsigned long _value)
{
    char buff[32];
	int len = snprintf(buff, 32, "%lu", _value);
	return OperateString(buff, len);
}

inline TRACE_LOG_BUFFER& TRACE_LOG_BUFFER::operator <<(unsigned char _value)
{
    return operator << ((long)_value);
}

inline TRACE_LOG_BUFFER& TRACE_LOG_BUFFER::operator <<(unsigned short _value)
{
    return operator << ((long)_value);
}

inline TRACE_LOG_BUFFER& TRACE_LOG_BUFFER::operator <<(unsigned int _value)
{
   return operator << ((unsigned long)_value);
}

inline TRACE_LOG_BUFFER& TRACE_LOG_BUFFER::operator <<(long long _value)
{
    char buff[32];
    int len = snprintf(buff, 32, "%lld", _value);
	return OperateString(buff, len);
}

inline TRACE_LOG_BUFFER& TRACE_LOG_BUFFER::operator <<(unsigned long long _value)
{
    char buff[32];
    int len = snprintf(buff, 32, "%lld", _value);
	return OperateString(buff, len);
}

inline TRACE_LOG_BUFFER& TRACE_LOG_BUFFER::operator <<(float _value)
{
    char buff[64];
	int len = snprintf(buff, 64, "%f", _value);
	return OperateString(buff, len);
}

inline TRACE_LOG_BUFFER& TRACE_LOG_BUFFER::operator <<(double _value)
{
    char buff[64];
	int len = snprintf(buff, 64, "%lf", _value);
	return OperateString(buff, len);
}
#ifdef _USE_STD_STRING
inline TRACE_LOG_BUFFER& TRACE_LOG_BUFFER::operator <<(string& _value)
{
    return operator << ((const char*)_value.c_str());
}

inline TRACE_LOG_BUFFER& TRACE_LOG_BUFFER::operator <<(const string& _value)
{
    return operator << ((const char*)_value.c_str());
}

inline TRACE_LOG_BUFFER& TRACE_LOG_BUFFER::operator <<(wstring& _value)
{
    return operator << ((const wchar_t*)_value.c_str());
}

inline TRACE_LOG_BUFFER& TRACE_LOG_BUFFER::operator <<(const wstring& _value)
{
    return operator << ((const wchar_t*)_value.c_str());
}
#endif
inline TRACE_LOG_BUFFER& TRACE_LOG_BUFFER::operator <<(const IN_ADDRINFO& _value)
{
	char buff[16];
	int len = snprintf(buff,16,"%d.%d.%d.%d",
                       _value.S_un.S_un_b.s_b1, _value.S_un.S_un_b.s_b2,
                       _value.S_un.S_un_b.s_b3, _value.S_un.S_un_b.s_b4);
	return OperateString(buff, len);
}

inline TRACE_LOG_BUFFER& TRACE_LOG_BUFFER::operator <<(const GUID& _value)
{
	//{00000000-0000-0000-0002-000000000000}
	char buff[64];
	int len = snprintf(buff,64,"{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}",
        _value.Data1, _value.Data2,_value.Data3,
		_value.Data4[0],_value.Data4[1],
		_value.Data4[2],_value.Data4[3],
		_value.Data4[4],_value.Data4[5],
		_value.Data4[6],_value.Data4[7]);
	return OperateString(buff, len);
}
//////////////////////////////////////////////////////////////////
extern TRACE_LOG_BUFFER* CDECL BeginTraceBuffer(int type, int level, const char* func_name,
                                                const char* source_file, int lineno);
extern void CDECL EndTraceBuffer(int type, TRACE_LOG_BUFFER* tace_buff);
//////////////////////////////////////////////////////////////////

#define tlbout(x) ((TRACE_LOG_BUFFER&)(*(x)))

#define TRACE_LOG_OUT0(type, level, file, func, line, value) \
    if (IsRequiredLevel(level))\
    {\
        TRACE_LOG_BUFFER* trace_buff = BeginTraceBuffer(type, level, file, func, line);\
        if(NULL != trace_buff)\
        {\
            tlbout(trace_buff) << value;\
            EndTraceBuffer(type, trace_buff);\
        }\
    }

#define TRACE_LOG_OUT1(value) \
    TRACE_LOG_OUT0(LOG_DISPLAYTYPE_NONE, TRACE_LOG_CURRENT_LEVEL, __FILE__, __FUNCTION__, __LINE__, str)

#define TRACE_LOG_OUT2(level, value) \
    TRACE_LOG_OUT0(LOG_DISPLAYTYPE_NONE, level, __FILE__, __FUNCTION__, __LINE__, value)

#define TRACE_LOG_OUT3(level, func, value) \
    TRACE_LOG_OUT0(LOG_DISPLAYTYPE_NONE, level, __FILE__, func, __LINE__, value)

#define TRACE_LOG_OUT4(type, level, func, value) \
    TRACE_LOG_OUT0(type, level, __FILE__, func, __LINE__, value)

#define TRACE_LOG0(x) TRACE_LOG_OUT2(LOG_LEVEL_EMERG, x)
#define TRACE_LOG1(x) TRACE_LOG_OUT2(LOG_LEVEL_ALERT, x)
#define TRACE_LOG2(x) TRACE_LOG_OUT2(LOG_LEVEL_CRIT, x)
#define TRACE_LOG3(x) TRACE_LOG_OUT2(LOG_LEVEL_ERROR, x)
#define TRACE_LOG4(x) TRACE_LOG_OUT2(LOG_LEVEL_WARNING, x)
#define TRACE_LOG5(x) TRACE_LOG_OUT2(LOG_LEVEL_NOTICE, x)
#define TRACE_LOG6(x) TRACE_LOG_OUT2(LOG_LEVEL_INFO, x)
#define TRACE_LOG7(x) TRACE_LOG_OUT2(LOG_LEVEL_DEBUG, x)

#define TRACE_LOG_EMERG(x)   TRACE_LOG0(x)
#define TRACE_LOG_ALERT(x)   TRACE_LOG1(x)
#define TRACE_LOG_CRIT(x)    TRACE_LOG2(x)
#define TRACE_LOG_ERROR(x)   TRACE_LOG3(x)
#define TRACE_LOG_WARNING(x) TRACE_LOG4(x)
#define TRACE_LOG_NOTICE(x)  TRACE_LOG5(x)
#define TRACE_LOG_INFO(x)    TRACE_LOG6(x)
#define TRACE_LOG_DEBUG(x)   TRACE_LOG7(x)
#define TRACE_LOG(x)         TRACE_LOG_OUT1(x)

#endif //__FCTARCELOG_H_
