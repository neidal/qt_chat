#ifndef __TRACE_THREAD_H
#define __TRACE_THREAD_H

#include "fctracelog.h"
#include "fcthread.h"
#include "fcclock.h"
#include <list>
using namespace std;
//#pragma warning(disable: 4786)

//背景写日志线程
class CTraceThread : public OFC::CThread
{
public:
	CTraceThread();

	void Init(int type,int level,
		const char* savepath, const char* filename,
		PTRACELOG_CALLBACK callback = NULL,
		bool show_srcfile = false,
		unsigned int max_cache = MAX_LOGCACHE_COUNT,
		unsigned int max_line = MAX_LOG_LINE,
		unsigned int max_size = MAX_LOG_SIZE);

	PTRACE_LOG_BUFFER WriteBegin(int type, int level, const char* srcfile,
                                 const char* func_name, int lineno);
	void WriteEnd(int type, TRACE_LOG_BUFFER* trace_buff);
	void Write(int type, int level,
               const char* srcfile, const char* func_name,
               int lineno, const char* format, va_list args);
	void Close();

	inline int GetLevel();
	inline void SetLevel(int level);
	inline bool IsRequiredLevel(int level);
	inline int GetType();
	inline void SetType(int type);
	inline bool GetShowSrcfileInfo();
	inline void SetShowSrcfileInfo(bool show_srcfile);
	inline void SetTraceCallback(PTRACELOG_CALLBACK callback);
	inline void SetMaxLogLine(unsigned int max_line = MAX_LOG_LINE);
	inline void SetMaxLogSize(unsigned int max_size = MAX_LOG_SIZE);

	static CTraceThread* GetInstance();
	static CTraceThread* Create();
	static void Destroy();
	static CTraceThread* SetInstance(CTraceThread* trace_thread);
private:
	bool CreateNewLogFile();
	char* GetLevelColor(int level);
	char GetLevelChar(int level);
	void ProcessLogBuffer();

public:
	virtual ~CTraceThread();
	//业务运行函数
	virtual void Run();
	//线程结束
	virtual VOID End();
	/* 终止线程 */
	virtual void Terminate();
private:
    struct LOG_DATE
    {
        int day;		/* Day.		[1-31] */
        int month;		/* Month.	[0-11] */
        int year;		/* Year	- 1900.    */

        bool operator != (const LOG_DATE& _log_date)
        {
            if (day != _log_date.day
                || month != _log_date.month
                || year != _log_date.year)
                return true;
            return false;
        }

        bool IsEqualDate(int year, int mon, int mday)
        {
            if (day != mday || month != mon || year != year)
                return false;
            return true;
        }
    };

	int type_;
	int level_;
	bool show_srcfile_;//是否显示源文件信息
	char* filename_;
	char* savepath_;
    bool isinit_;
    LOG_DATE date_;
	unsigned int line_;
    unsigned int size_;
    unsigned int max_size_;
    unsigned int max_line_;
	FILE* file_;
	PTRACELOG_CALLBACK log_callback_;

	OFC::CCriticalSection lock_;
	std::list<PTRACE_LOG_BUFFER> log_buffer_list_;
	OFC::CEvent wait_buffer_;

	static CTraceThread* trace_thread_;
	static bool alone_trace_;
};
//////////////////////////////////////////////////////////////////////////
//inline

inline int CTraceThread::GetLevel()
{
	return level_;
}

inline void CTraceThread::SetLevel(int level)
{
    level_ = level;
}

inline bool CTraceThread::IsRequiredLevel(int level)
{
    if (level > level_ || !(LOG_DISPLAYTYPE_SHOW & type_))
        return false;
    return true;
}

inline int CTraceThread::GetType()
{
	return type_;
}

inline void CTraceThread::SetType(int type)
{
	type_ = type;
}

inline bool CTraceThread::GetShowSrcfileInfo()
{
	return show_srcfile_;
}

inline void CTraceThread::SetShowSrcfileInfo(bool show_srcfile)
{
	show_srcfile_ = show_srcfile;
}

inline void CTraceThread::SetTraceCallback(PTRACELOG_CALLBACK callback)
{
	log_callback_ = callback;
}

inline void CTraceThread::SetMaxLogLine(unsigned int max_line)
{
    max_line_ = max_line;
}

inline void CTraceThread::SetMaxLogSize(unsigned int max_size)
{
    max_size_ = max_size;
}
#endif //__TRACE_THREAD_H
