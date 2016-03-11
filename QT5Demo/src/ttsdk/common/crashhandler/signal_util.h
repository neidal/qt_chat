/*
 * signal_util.h
 *
 *  Created on: 2013Äê8ÔÂ9ÈÕ
 *      Author: liaofeng
 */


#ifndef SIGNAL_UTIL_H_
#define SIGNAL_UTIL_H_

#include <signal.h>
#include <asm/sigcontext.h>

typedef struct sigcontext mcontext_t;

typedef struct ucontext {
	uint32_t uc_flags;
	struct ucontext* uc_link;
	stack_t uc_stack;
	mcontext_t uc_mcontext;
} ucontext_t;

struct xlat {
	int val;
	const char *str;
};

static const struct xlat sig_codes[] = {
	{ SIGILL,		"SIGSEGV"		},
	{ SIGFPE,		"SIGSEGV"		},
	{ SIGSEGV,		"SIGSEGV"		},
	{ SIGBUS,		"SIGSEGV"		},
	{ 0,			NULL			},
};

static const struct xlat sigill_codes[] = {
	{ ILL_ILLOPC,	"ILL_ILLOPC"	},
	{ ILL_ILLOPN,	"ILL_ILLOPN"	},
	{ ILL_ILLADR,	"ILL_ILLADR"	},
	{ ILL_ILLTRP,	"ILL_ILLTRP"	},
	{ ILL_PRVOPC,	"ILL_PRVOPC"	},
	{ ILL_PRVREG,	"ILL_PRVREG"	},
	{ ILL_COPROC,	"ILL_COPROC"	},
	{ ILL_BADSTK,	"ILL_BADSTK"	},
	{ 0,			NULL			},
};

static const struct xlat sigfpe_codes[] = {
	{ FPE_INTDIV,	"FPE_INTDIV"	},
	{ FPE_INTOVF,	"FPE_INTOVF"	},
	{ FPE_FLTDIV,	"FPE_FLTDIV"	},
	{ FPE_FLTOVF,	"FPE_FLTOVF"	},
	{ FPE_FLTUND,	"FPE_FLTUND"	},
	{ FPE_FLTRES,	"FPE_FLTRES"	},
	{ FPE_FLTINV,	"FPE_FLTINV"	},
	{ FPE_FLTSUB,	"FPE_FLTSUB"	},
	{ 0,			NULL			},
};

static const struct xlat sigsegv_codes[] = {
	{ SEGV_MAPERR,	"SEGV_MAPERR"	},
	{ SEGV_ACCERR,	"SEGV_ACCERR"	},
	{ 0,			NULL			},
};

static const struct xlat sigbus_codes[] = {
	{ BUS_ADRALN,	"BUS_ADRALN"	},
	{ BUS_ADRERR,	"BUS_ADRERR"	},
	{ BUS_OBJERR,	"BUS_OBJERR"	},
	{ 0,			NULL			},
};

const char * xlookup(const struct xlat *xlat, int val)
{
	for (; xlat->str != NULL; xlat++)
		if (xlat->val == val)
			return xlat->str;
	return NULL;
}

const char * getSignalName(int sig)
{
	return xlookup(sig_codes, sig);
}

const char * getSignalCodeName(int sig, int code)
{
	switch (sig) {
	case SIGILL:
		return xlookup(sigill_codes, code);
		break;
	case SIGFPE:
		return xlookup(sigfpe_codes, code);
		break;
	case SIGSEGV:
		return xlookup(sigsegv_codes, code);
		break;
	case SIGBUS:
		return xlookup(sigbus_codes, code);
		break;
	default:
		return NULL;
	}
}


#endif /* SIGNAL_UTIL_H_ */
