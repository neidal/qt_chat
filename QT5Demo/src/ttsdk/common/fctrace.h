#ifndef __FCTRACE_H_
#define __FCTRACE_H_

#include <stdarg.h>
#include "fcdefs.h"

#ifndef __FUNCTION__
#define __FUNCTION__ __PRETTY_FUNCTION__
#endif

//��־��ʾ��ʽType
#define LOG_DISPLAYTYPE_NONE        (0x00)			// ����ʾ��־
#define LOG_DISPLAYTYPE_SHOW        (0x01)           // ��ʾ��־
#define LOG_DISPLAYTYPE_FILE        (0x02)			// ���ļ���ʽ
#define LOG_DISPLAYTYPE_THREAD      (0x04)			// �����߳�д��־��ʽ
#define LOG_DISPLAYTYPE_CONSOLE     (0x08)			// ����̨��ʽ
#define LOG_DISPLAYTYPE_COLOR		(0x10)			// �Ƿ����ɫ���

#if defined(__ANDROID__) || defined(__APPLE__)
#ifdef DEBUG
#define DEF_LOG_DISPLAYTYPE			(LOG_DISPLAYTYPE_SHOW | LOG_DISPLAYTYPE_THREAD | LOG_DISPLAYTYPE_FILE)
#else
#define DEF_LOG_DISPLAYTYPE			(LOG_DISPLAYTYPE_SHOW)
#endif
#elif defined(_WIN32)
#define DEF_LOG_DISPLAYTYPE			(LOG_DISPLAYTYPE_SHOW | LOG_DISPLAYTYPE_THREAD | LOG_DISPLAYTYPE_FILE)
#else
#define DEF_LOG_DISPLAYTYPE			(LOG_DISPLAYTYPE_SHOW | LOG_DISPLAYTYPE_THREAD | LOG_DISPLAYTYPE_FILE | LOG_DISPLAYTYPE_COLOR)
#endif

//��־�ȼ�Level
/** ϵͳ��������־*/
#define LOG_LEVEL_EMERG       (0)
/* Σ�մ����п���Σ��ϵͳ  */
#define LOG_LEVEL_ALERT       (1)
/** ���ܺ��Ե����ش��� */
#define LOG_LEVEL_CRIT        (2)
/** �����ǲ��ܺ��Ե� */
#define LOG_LEVEL_ERROR       (3)
/** �����������Ǻ���Ҫ,���Ժ��Ե� */
#define LOG_LEVEL_WARNING     (4)
/** ֵ��ע������� */
#define LOG_LEVEL_NOTICE      (5)
/** ��ͨ��־����ʾ��Ϣ */
#define LOG_LEVEL_INFO        (6)
/** ������ǳ�����ϸ��*/
#define LOG_LEVEL_DEBUG       (7)
#define LOG_LEVEL_MAX         (8)

//�ɼ�¼ѡ��
#define LOG_CAN_LEVEL          (0x1)
#define LOG_CAN_FILELINE       (0x2)
#define LOG_CAN_THREAD         (0x4)

#ifndef MAX_LOGCACHE_COUNT
#define MAX_LOGCACHE_COUNT     10000            // 10M������־����
#endif

#ifndef MAX_LOG_SIZE
#define MAX_LOG_SIZE           (10*1024*1024)   //���10M��־
#endif

#ifndef MAX_LOG_LINE
#define MAX_LOG_LINE           100000           // һ����־�ļ�����¼����100000Լ100MB�ļ���С��
#endif

#ifdef _DEBUG
#define TRACE_LOG_CURRENT_LEVEL LOG_LEVEL_WARNING
#else
#define TRACE_LOG_CURRENT_LEVEL LOG_LEVEL_ERROR
#endif

typedef void (FCAPI * PTRACELOG_CALLBACK)(int level, const char* buf, int len);

//////////////////////////////////////////////////////////////////
//����������������ͬ������������ģ���еĵ��ü�¼��־��
extern void CDECL SetTraceInstance(void* trace_instance);
extern void* CDECL GetTraceInstance();
//////////////////////////////////////////////////////////////////
extern void CDECL InitTrace(int type, int level,
			   const char* savepath, const char* filename,
			   PTRACELOG_CALLBACK callback = NULL,
			   bool show_srcfile = false,
               unsigned int max_cache = MAX_LOGCACHE_COUNT,
               unsigned int max_line = MAX_LOG_LINE,
               unsigned int max_size = MAX_LOG_SIZE);

extern void CDECL SetTraceLevel(int level);
extern bool CDECL IsRequiredLevel(int level);
extern void CDECL SetTraceType(int type);
extern void CDECL SetTraceShowSrcfileInfo(bool show_srcfile);
extern void CDECL SetMaxTraceLine(unsigned int max_line = MAX_LOG_LINE);
extern void CDECL SetMaxTraceSize(unsigned int max_size = MAX_LOG_SIZE);
extern void CDECL Trace0(int type, int level,
                  const char* srcfile, const char* func_name,
                  int lineno, const char* format, va_list args);
extern void CDECL CloseTrace();
//////////////////////////////////////////////////////////////////
fcinline void CDECL Trace(int type, int level,
                        const char* srcfile, const char* func_name,
                        int lineno, const char* format, ...)
{
	if (IsRequiredLevel(level))
	{
		va_list args;
        va_start(args, format);
		Trace0(type, level, srcfile, func_name, lineno, format, args);
		va_end(args);
	}
}

class CTraceFileAndLineInfo
{
public:
	CTraceFileAndLineInfo(const char *filename, const char *func_name,
                          int lineno, int level = TRACE_LOG_CURRENT_LEVEL)
		: filename_(filename), func_name_(func_name),
		  lineno_(lineno), level_(level)
	{}

	fcinline void CDECL operator()(int type, int level, const char *format, ...) const
	{
		if (IsRequiredLevel(level))
		{
            va_list args;
            va_start(args, format);
            Trace0(type, level, filename_, func_name_, lineno_, format, args);
            va_end(args);
        }
	}
	fcinline void CDECL operator()(int level, const char *format, ...) const
	{
		if (IsRequiredLevel(level))
		{
            va_list args;
            va_start(args, format);
            Trace0(LOG_DISPLAYTYPE_NONE, level, filename_, func_name_, lineno_, format, args);
            va_end(args);
        }
	}
	fcinline void CDECL operator()(const char *format, ...) const
	{
		if (IsRequiredLevel(TRACE_LOG_CURRENT_LEVEL))
		{
            va_list args;
            va_start(args, format);
            Trace0(LOG_DISPLAYTYPE_NONE, level_, filename_, func_name_, lineno_, format, args);
            va_end(args);
        }
	}

private:
	const char * const filename_;
    const char* const func_name_;
	const int lineno_;
	const int level_;

    DISALLOW_COPY_AND_ASSIGN(CTraceFileAndLineInfo);
};
//////////////////////////////////////////////////////////////////
#define INIT_TRACE ::InitTrace
#define SET_TRACE_LEVEL ::SetTraceLevel
#define SET_TRACE_TYPE ::SetTraceType
#define SET_TRACE_SHOWSRCFILEINFO ::SetTraceShowSrcfileInfo
#define SET_TRACE_MAXLINE ::SetMaxTraceLine
#define SET_TRACE_MAXSIZE ::SetMaxTraceSize
#define CLOSE_TRACE ::CloseTrace

#ifndef TRACE
#ifdef _DEBUG
#define TRACE CTraceFileAndLineInfo(__FILE__, __FUNCTION__, __LINE__)
#else
#define TRACE
#endif
#endif

#ifdef _DEBUG
#define TRACE_LEVEL(x) if (IsRequiredLevel(x)) CTraceFileAndLineInfo(__FILE__, __FUNCTION__, __LINE__, x)
#else
#define TRACE_LEVEL(x) if (IsRequiredLevel(x)) CTraceFileAndLineInfo(NULL, NULL, -1, x)
#endif
#define TRACE_EMERG   TRACE_LEVEL(LOG_LEVEL_EMERG)
#define TRACE_ALERT   TRACE_LEVEL(LOG_LEVEL_ALERT)
#define TRACE_CRIT    TRACE_LEVEL(LOG_LEVEL_CRIT)
#define TRACE_ERROR   TRACE_LEVEL(LOG_LEVEL_ERROR)
#define TRACE_WARNING TRACE_LEVEL(LOG_LEVEL_WARNING)
#define TRACE_NOTICE  TRACE_LEVEL(LOG_LEVEL_NOTICE)
#define TRACE_INFO    TRACE_LEVEL(LOG_LEVEL_INFO)
#define TRACE_DEBUG   TRACE_LEVEL(LOG_LEVEL_DEBUG)

//WANGGENG CHANGE NOTE assert
#include <assert.h>
#ifndef ASSERT
#ifdef _DEBUG
#define ASSERT(f) assert(f)
#else
#define ASSERT(f)
#endif
#endif

#ifndef THROWEXCEPTION
#include <exception>
#define THROWEXCEPTION(x) throw std::exception()
#endif //THROWEXCEPTION

#endif //__FCTRACE_H_
