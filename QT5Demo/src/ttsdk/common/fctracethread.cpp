#include <sys/timeb.h>
#include "fcsystem.h"
#include "fctracethread.h"

#ifdef _WIN32
#define localtime_r(a,b)	localtime_s(b,a)
#endif

#define LOG_COLOR_MAX  (7)
char LOG_COLOR_CHARARRAY[LOG_COLOR_MAX][9]={   //color for linux
    "\033[1;30m", //GRAY
    "\033[1;33m", //YELLOW
    "\033[1;35m", //PINK
    "\033[1;31m", //RED
    "\033[1;36m", //BLUE
    "\033[1;37m", //WHITE
    "\033[1;32m", //GREEN
};

const char LOG_LEVEL_CHARARRAY[LOG_LEVEL_MAX+2] = "&=+@!$*#%";

//CTraceThread _trace_thread;

CTraceThread* CTraceThread::trace_thread_ = NULL;
bool CTraceThread::alone_trace_ = false;

CTraceThread* CTraceThread::GetInstance()
{
	return trace_thread_;
}

CTraceThread* CTraceThread::Create()
{
	if (!trace_thread_)
	{
		CTraceThread* trace_thread = new CTraceThread();
		if (trace_thread)
		{
			alone_trace_ = true;
			trace_thread_ = trace_thread;
		}
	}
	return trace_thread_;
}

void CTraceThread::Destroy()
{
	if (trace_thread_ && alone_trace_)
	{
		CTraceThread* trace_thread = trace_thread_;
		trace_thread_ = NULL;
		delete trace_thread;
	}
	trace_thread_ = NULL;
	alone_trace_ = false;
}

CTraceThread* CTraceThread::SetInstance(CTraceThread* trace_thread)
{
	Destroy();
	trace_thread_ = trace_thread;
	return trace_thread_;
}

CTraceThread::CTraceThread()
{
	type_ = DEF_LOG_DISPLAYTYPE;
    level_ = LOG_LEVEL_NOTICE;
    show_srcfile_ = false;
    ZeroMemory(&date_, sizeof(date_));
    isinit_ = false;
    line_ = 0;
    size_ = 0;
    max_line_ = MAX_LOG_LINE;
    max_size_ = MAX_LOG_SIZE;
	filename_ = NULL;
	savepath_ = NULL;
	log_callback_ = NULL;
	file_ = NULL;
	wait_buffer_.Create(false, false);
//#ifdef __APPLE__
//	char waitname[64];
//	int len = sprintf(waitname, "tracelog_waitsem_%u", (ULONG_PTR)(&wait_buffer_));
//	waitname[len] = 0;
//	wait_buffer_.Open(waitname);
//#endif
}

CTraceThread::~CTraceThread()
{
	type_ = LOG_DISPLAYTYPE_NONE;
	level_ = LOG_LEVEL_EMERG;
	Stop();
	log_callback_ = NULL;
	show_srcfile_ = false;
	if (filename_)
	{
		delete[] filename_;
		filename_ = NULL;
	}

	if (savepath_)
	{
		delete[] savepath_;
		savepath_ = NULL;
	}

	if (file_)
	{
		::fclose(file_);
		file_ = NULL;
	}

	//需要清空日志缓存列表
	PTRACE_LOG_BUFFER trace_buffer = NULL;
	std::list<PTRACE_LOG_BUFFER>::iterator itr = log_buffer_list_.begin();
	for (; itr != log_buffer_list_.end(); itr++)
	{
		trace_buffer = *itr;
		if (trace_buffer)
		{
			delete trace_buffer;
			trace_buffer = NULL;
		}
	}
}

//日志目录结构
//-日志根目录
//---年
//---月
//---日
//-----%s_%d.log
bool CTraceThread::CreateNewLogFile()
{
	if (file_)
	{
		::fclose(file_);
		file_ = NULL;
	}

    struct timeb tmb;
    struct tm ltm;
    ftime(&tmb);
	localtime_r(&tmb.time, &ltm);

	int len = 0;
	char filename[MAX_PATH];
	if (savepath_ && savepath_[0] != 0)
	{
		len = snprintf(filename, MAX_PATH, "%s", savepath_);
		filename[len] = 0;
		filename[len] = CSystem::FillDirSeparator(filename, len);
		if (filename[len] != 0)
			filename[len + 1] = 0;
	}
	else
	{
		len = CSystem::GetAppPath(filename, MAX_PATH);
		filename[len] = 0;
	}

    len += snprintf(filename + len, MAX_PATH - len, "log/%d/%d/%d",
                       ltm.tm_year+1900, ltm.tm_mon+1, ltm.tm_mday);
    filename[len] = 0;
    CSystem::MakeDirectory(filename);
	if (filename_)
	{
		len += snprintf(filename + len, MAX_PATH - len,
			"/%s_%02d%02d%02d%03d.log", filename_, ltm.tm_hour,
			ltm.tm_min, ltm.tm_sec, tmb.millitm);
	}
	else
	{
		len += snprintf(filename + len, MAX_PATH - len,
			"/%02d%02d%02d%03d.log", ltm.tm_hour,
			ltm.tm_min, ltm.tm_sec, tmb.millitm);
	}
    filename[len] = 0;

	file_ = ::fopen(filename, "ab");
	//if (file_)
	//{
	//	::fseek(file_, 0, SEEK_SET);
	//}
	return file_ ? true : false;
}

char* CTraceThread::GetLevelColor(int level)
{
    int color_index = 0;
    if ((level >= 0) && (level < LOG_COLOR_MAX) )
    {
        color_index = level;
    }
    else
    {
        color_index = LOG_COLOR_MAX - 1;    //Default WHITE Color
    }
    return LOG_COLOR_CHARARRAY[color_index];
}

char CTraceThread::GetLevelChar(int level)
{
    int char_index = 0;
    if ((level >= 0) && (level < LOG_LEVEL_MAX) )
    {
        char_index = level;
    }
    else
    {
        char_index = LOG_LEVEL_MAX;         //Defualt '%' char
    }
    return LOG_LEVEL_CHARARRAY[char_index];
}

void CTraceThread::Init(int type, int level,
						const char* savepath, const char* filename,
						PTRACELOG_CALLBACK callback,
                        bool show_srcfile, unsigned int max_cache,
                        unsigned int max_line, unsigned int max_size)
{
	if (!alone_trace_ || isinit_)
        return;

    if (level <= LOG_LEVEL_MAX)
        level_ = level;

    type_ = type;
    show_srcfile_ = show_srcfile;
	log_callback_ = callback;
	if (filename_)
	{
		delete[] filename_;
		filename_ = NULL;
	}
	if (savepath_)
	{
		delete[] savepath_;
		savepath_ = NULL;
	}

	int len = 0;
	if (filename)
	{
		len = strlen(filename) + 1;
		filename_ = new char[len];
		if (filename_)
			strncpy(filename_, filename, len);
	}
	if (savepath)
	{
		len = strlen(savepath) + 1;
		savepath_ = new char[len];
		if (savepath_)
			strncpy(savepath_, savepath, len);
	}

    //buff_pool_.init(max_cache);
    line_ = 0;
    max_line_ = max_line;
    max_size_ = max_size;
    if (type_ & LOG_DISPLAYTYPE_SHOW)
    {
        if (type_ & LOG_DISPLAYTYPE_THREAD)
        {
            isinit_ = Start();
            return;
        }
    }
    isinit_ = true;
}

TRACE_LOG_BUFFER* CTraceThread::WriteBegin(int type, int level,
        const char* srcfile, const char* func_name, int lineno)
{
	type &= ~LOG_DISPLAYTYPE_THREAD;
	type |= type_;
	if (level > level_ || !(LOG_DISPLAYTYPE_SHOW & type))
        return NULL;

	TRACE_LOG_BUFFER* trace_buff = new TRACE_LOG_BUFFER;// buff_pool_.malloc();
    if(!trace_buff)
        return NULL;

    //trace_buff->Init();
	trace_buff->log_level = level;
	trace_buff->log_type = type;

    struct timeb tmb;
    struct tm ltm;
    ftime(&tmb);
	localtime_r(&tmb.time, &ltm);

    //若日期不一样，则也重新创建日志文件
	if ((LOG_DISPLAYTYPE_FILE & type)
		&& !date_.IsEqualDate(ltm.tm_year, ltm.tm_mon, ltm.tm_mday))
    {
        date_.year = ltm.tm_year;
        date_.month = ltm.tm_mon;
        date_.day = ltm.tm_mday;
        CreateNewLogFile();
    }

    int len = 0;
	int max_len = sizeof(trace_buff->log_buff);
	len = snprintf(trace_buff->log_buff, max_len, "%c-%d  %d-%d ",
		GetLevelChar(level), level,	CSystem::GetProccessID(), CSystem::GetThreadID());

	if (LOG_DISPLAYTYPE_COLOR & type)
		len += snprintf(trace_buff->log_buff + len, max_len - len, "%s", GetLevelColor(level));

	len += snprintf(trace_buff->log_buff + len, max_len - len, "%04d-%02d-%02d %02d:%02d:%02d.%03d",
		ltm.tm_year + 1900, ltm.tm_mon + 1, ltm.tm_mday,
		ltm.tm_hour, ltm.tm_min,
		ltm.tm_sec, tmb.millitm);

	if (show_srcfile_ && srcfile != NULL && func_name != NULL && lineno != -1)
		len += snprintf(trace_buff->log_buff + len, max_len - len, " %s(%d)[%s] ", srcfile, lineno, func_name);

    //if(show_srcfile_ && srcfile != NULL && func_name != NULL && lineno != -1)
    //{
    //    len = snprintf(trace_buff->log_buff,SINGLELOG_BUFFER_SIZE,
    //                     "%c-%d  %d-%d  %s%04d-%02d-%02d %02d:%02d:%02d.%03d %s(%d)[%s]: ",
    //                     GetLevelChar(level), level,
    //                     CSystem::GetProccessID(), CSystem::GetThreadID(),
    //                     GetLevelColor(level),
    //                     ltm.tm_year+1900, ltm.tm_mon+1, ltm.tm_mday,
    //                     ltm.tm_hour, ltm.tm_min,
    //                     ltm.tm_sec, tmb.millitm,
    //                     srcfile, lineno, func_name);
    //}
    //else
    //{
    //    len = snprintf(trace_buff->log_buff,SINGLELOG_BUFFER_SIZE,
    //                     "%c-%d  %d-%d  %s%04d-%02d-%02d %02d:%02d:%02d.%03d: ",
    //                     GetLevelChar(level), level,
    //                     CSystem::GetProccessID(), CSystem::GetThreadID(),
    //                     GetLevelColor(level),
    //                     ltm.tm_year+1900, ltm.tm_mon+1, ltm.tm_mday,
    //                     ltm.tm_hour, ltm.tm_min,
    //                     ltm.tm_sec, tmb.millitm);
    //}

    if (len <= 0)
    {
        //buff_pool_.free(trace_buff);
		delete trace_buff;
		trace_buff = NULL;
        return NULL;
    }

	trace_buff->log_buff[len] = ':';
	++len;
    trace_buff->log_head_size = len;
    trace_buff->log_size = len;
    trace_buff->log_buff[len] = '\0';
    return trace_buff;
}

void CTraceThread::WriteEnd(int type, TRACE_LOG_BUFFER* trace_buff)
{
    if(!trace_buff)
        return;

    (*trace_buff) <<"\n";
	type &= ~LOG_DISPLAYTYPE_THREAD;
	type |= type_;
	trace_buff->log_type = type;
    if (!(type & LOG_DISPLAYTYPE_SHOW))
    {
		delete trace_buff;
		trace_buff = NULL;
        //buff_pool_.free(trace_buff);
		return;
    }

    //if (LOG_DISPLAYTYPE_CONSOLE & type)
    //    printf((char*)(trace_buff->log_buff + trace_buff->log_head_size));

    if (LOG_DISPLAYTYPE_THREAD & type)
    {
        //buff_pool_.push(trace_buff);
		//TODO push buff list
		lock_.Lock();
		log_buffer_list_.push_back(trace_buff);
		lock_.Unlock();
		wait_buffer_.SetEvent();
    }
    else
    {
		//buff_pool_.lock();
		line_++;
		size_ += trace_buff->log_size;
		if (log_callback_)
		{
			log_callback_(trace_buff->log_level, trace_buff->log_buff, trace_buff->log_size);
		}

		if ((LOG_DISPLAYTYPE_FILE & type) && file_)
		{
			::fwrite(trace_buff->log_buff, sizeof(char), trace_buff->log_size, file_);
			::fflush(file_);
			if (line_ >= max_line_ || size_ >= max_size_)
			{
				line_ = 0;
				size_ = 0;
				//生成新的TRACE文件名
				CreateNewLogFile();
			}
		}
		//buff_pool_.unlock();
		//buff_pool_.free(trace_buff);
		delete trace_buff;
		trace_buff = NULL;
    }
}

void CTraceThread::Write(int type, int level,
                         const char* srcfile, const char* func_name,
                         int lineno, const char* pszFmt, va_list args)
{
	type &= ~LOG_DISPLAYTYPE_THREAD;
    type |= type_;
    if (level > level_ || !(type & LOG_DISPLAYTYPE_SHOW))
        return;

	TRACE_LOG_BUFFER* trace_buff = new TRACE_LOG_BUFFER;// buff_pool_.malloc();
    if(!trace_buff)
        return;

    //trace_buff->Init();
	trace_buff->log_level = level;
	trace_buff->log_type = type;

    struct timeb tmb;
    struct tm ltm;
    ftime(&tmb);
	localtime_r(&tmb.time, &ltm);

    //若日期不一样，则也重新创建日志文件
	if ((LOG_DISPLAYTYPE_FILE & type)
		&& !date_.IsEqualDate(ltm.tm_year, ltm.tm_mon, ltm.tm_mday))
    {
        date_.year = ltm.tm_year;
        date_.month = ltm.tm_mon;
        date_.day = ltm.tm_mday;
        line_ = 0;
        size_ = 0;
        CreateNewLogFile();
    }

	int len = 0;
	int max_len = sizeof(trace_buff->log_buff);
	len = snprintf(trace_buff->log_buff, max_len, "%c-%d  %d-%d ",
		GetLevelChar(level), level, CSystem::GetProccessID(), CSystem::GetThreadID());

	if (LOG_DISPLAYTYPE_COLOR & type)
		len += snprintf(trace_buff->log_buff + len, max_len - len, "%s", GetLevelColor(level));

	len += snprintf(trace_buff->log_buff + len, max_len - len, "%04d-%02d-%02d %02d:%02d:%02d.%03d ",
		ltm.tm_year + 1900, ltm.tm_mon + 1, ltm.tm_mday,
		ltm.tm_hour, ltm.tm_min,
		ltm.tm_sec, tmb.millitm);

	if (show_srcfile_ && srcfile != NULL && func_name != NULL && lineno != -1)
		len += snprintf(trace_buff->log_buff + len, max_len - len, "%s(%d)[%s] ", srcfile, lineno, func_name);

    //if (show_srcfile_ && srcfile != NULL && func_name != NULL && lineno != -1)
    //{
    //    len = snprintf(trace_buff->log_buff, SINGLELOG_BUFFER_SIZE,
    //                     "%c-%d  %d-%d  %s%04d-%02d-%02d %02d:%02d:%02d.%03d %s(%d)[%s]: ",
    //                     GetLevelChar(level), level,
    //                     CSystem::GetProccessID(), CSystem::GetThreadID(),
    //                     GetLevelColor(level),
    //                     ltm.tm_year+1900, ltm.tm_mon+1, ltm.tm_mday,
    //                     ltm.tm_hour, ltm.tm_min,
    //                     ltm.tm_sec, tmb.millitm,
    //                     srcfile, lineno, func_name);
    //}
    //else
    //{
    //    len = snprintf(trace_buff->log_buff, SINGLELOG_BUFFER_SIZE,
    //                     "%c-%d  %d-%d  %s%04d-%02d-%02d %02d:%02d:%02d.%03d: ",
    //                     GetLevelChar(level), level,
    //                     CSystem::GetProccessID(), CSystem::GetThreadID(),
    //                     GetLevelColor(level),
    //                     ltm.tm_year+1900, ltm.tm_mon+1, ltm.tm_mday,
    //                     ltm.tm_hour, ltm.tm_min,
    //                     ltm.tm_sec, tmb.millitm);
    //}

    if (len <= 0)
    {
        //buff_pool_.free(trace_buff);
		delete trace_buff;
		trace_buff = NULL;
        return;
    }

	trace_buff->log_buff[len] = ':';
	++len;
    trace_buff->log_head_size = len;
	int lentmp = vsnprintf(trace_buff->log_buff + len, max_len - len, pszFmt, args);
	if (lentmp > 0 && lentmp < (max_len - len))
    {
        lentmp += len;
        //若没有结束符号时，自动加上结束符号
        if (trace_buff->log_buff[lentmp - 1] != '\n')
        {
			if (lentmp < (max_len - 1))
            {
                trace_buff->log_buff[lentmp] = '\n';
                lentmp ++;
            }
            else
            {
                trace_buff->log_buff[lentmp - 1] = '\n';
            }
        }
        trace_buff->log_size = lentmp;
        trace_buff->log_buff[lentmp] = '\0';

        //if (LOG_DISPLAYTYPE_CONSOLE & type)
        //    printf((char*)(trace_buff->log_buff + trace_buff->log_head_size));

        if (LOG_DISPLAYTYPE_THREAD & type)
        {
            //buff_pool_.push(trace_buff);
			lock_.Lock();
			log_buffer_list_.push_back(trace_buff);
			lock_.Unlock();
			wait_buffer_.SetEvent();
        }
		else
		{
			//buff_pool_.lock();
			line_++;
			size_ += trace_buff->log_size;
			if (log_callback_)
			{
				log_callback_(trace_buff->log_level, trace_buff->log_buff, trace_buff->log_size);
			}
			
			if ((LOG_DISPLAYTYPE_FILE & type) && file_)
			{
				fwrite(trace_buff->log_buff, sizeof(char), trace_buff->log_size, file_);
				::fflush(file_);
				if (line_ >= max_line_ || size_ >= max_size_)
				{
					line_ = 0;
					size_ = 0;
					//生成新的TRACE文件名
					CreateNewLogFile();
				}
			}
			//buff_pool_.unlock();
			//buff_pool_.free(trace_buff);
			delete trace_buff;
			trace_buff = NULL;
		}
    }
    else
    {
        //buff_pool_.free(trace_buff);
		delete trace_buff;
		trace_buff = NULL;
    }
}

void CTraceThread::ProcessLogBuffer()
{
	TRACE_LOG_BUFFER* trace_buff = NULL;
	while (log_buffer_list_.size() != 0)
	{
		lock_.Lock();
		//trace_buff = buff_pool_.pop();
		trace_buff = log_buffer_list_.front();
		log_buffer_list_.pop_front();
		lock_.Unlock();
		if (trace_buff != NULL)
		{
			line_++;
			size_ += trace_buff->log_size;

			if (log_callback_)
			{
				log_callback_(trace_buff->log_level, trace_buff->log_buff, trace_buff->log_size);
			}

			if ((LOG_DISPLAYTYPE_FILE & trace_buff->log_type) && file_)
			{
				::fwrite(trace_buff->log_buff, sizeof(char), trace_buff->log_size, file_);
				::fflush(file_);
				if (line_ >= max_line_ || size_ >= max_size_)
				{
					line_ = 0;
					size_ = 0;
					//生成新的TRACE文件名
					CreateNewLogFile();
				}
			}
			//buff_pool_.free(trace_buff);
			delete trace_buff;
			trace_buff = NULL;
		}
	}
}

void CTraceThread::Run()
{
	wait_buffer_.WaitForEvent();
	ProcessLogBuffer();
}

VOID CTraceThread::End()
{
	ProcessLogBuffer();
	if (file_)
	{
		::fclose(file_);
		file_ = NULL;
	}
	isinit_ = false;
	CThread::End();
}

void CTraceThread::Terminate()
{
	CThread::Terminate();
	wait_buffer_.SetEvent();
}

void CTraceThread::Close()
{
	type_ = LOG_DISPLAYTYPE_NONE;
	level_ = LOG_LEVEL_EMERG;
    Terminate();
}

/////////////////////////////////////////////////////////////////
void CDECL SetTraceInstance(void* trace_instance)
{
	CTraceThread::SetInstance((CTraceThread*)trace_instance);
}

void* CDECL GetTraceInstance()
{
	return (void*)CTraceThread::GetInstance();
}

void CDECL InitTrace(int type, int level,
					 const char* savepath, const char* filename,
					 PTRACELOG_CALLBACK callback, bool show_srcfile,
                     unsigned int max_cache, unsigned int max_line,
                     unsigned int max_size)
{
	CTraceThread::Create()->Init(type, level, savepath, filename, callback,
									  show_srcfile, max_cache, max_line, max_size);
}

void CDECL SetTraceLevel(int level)
{
	CTraceThread* trace_thread = CTraceThread::GetInstance();
	if (trace_thread)
		trace_thread->SetLevel(level);
}

bool CDECL IsRequiredLevel(int level)
{
	CTraceThread* trace_thread = CTraceThread::GetInstance();
	if (trace_thread)
		return trace_thread->IsRequiredLevel(level);
	return false;
}

void CDECL SetTraceType(int type)
{
	CTraceThread* trace_thread = CTraceThread::GetInstance();
	if (trace_thread)
		trace_thread->SetType(type);
}

void CDECL SetTraceShowSrcfileInfo(bool show_srcfile)
{
	CTraceThread* trace_thread = CTraceThread::GetInstance();
	if (trace_thread)
		trace_thread->SetShowSrcfileInfo(show_srcfile);
}

void CDECL SetMaxTraceLine(unsigned int max_line)
{
	CTraceThread* trace_thread = CTraceThread::GetInstance();
	if (trace_thread)
		trace_thread->SetMaxLogLine(max_line);
}

void CDECL SetMaxTraceSize(unsigned int max_size)
{
	CTraceThread* trace_thread = CTraceThread::GetInstance();
	if (trace_thread)
		trace_thread->SetMaxLogSize(max_size);
}

void CDECL Trace0(int type, int level,
                  const char* srcfile, const char* func_name,
                  int lineno, const char* format, va_list args)
{
	CTraceThread* trace_thread = CTraceThread::GetInstance();
	if (trace_thread)
		trace_thread->Write(type, level, srcfile, func_name, lineno, format, args);
}

TRACE_LOG_BUFFER* BeginTraceBuffer(int type, int level, const char* srcfile,
                                   const char* func_name, int lineno)
{
	CTraceThread* trace_thread = CTraceThread::GetInstance();
	if (!trace_thread)
		return NULL;

	return trace_thread->WriteBegin(type, level, srcfile, func_name, lineno);
}

void EndTraceBuffer(int type, TRACE_LOG_BUFFER* trace_buff)
{
	CTraceThread* trace_thread = CTraceThread::GetInstance();
	if (trace_thread)
		trace_thread->WriteEnd(type, trace_buff);
}

void CloseTrace()
{
	CTraceThread::Destroy();
}
