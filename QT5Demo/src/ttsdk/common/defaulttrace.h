#ifndef __TTDEFAULT_TRACE_H_
#define __TTDEFAULT_TRACE_H_

const char TAG[] = "TTSDK";

inline void FCAPI OFCTraceCallback(int level, const char* buf, int len)
{
    printf("%s %d %s", TAG, level, buf);
}

#endif //__TTDEFAULT_TRACE_H_

