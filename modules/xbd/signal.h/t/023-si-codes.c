/*
 * The si_code constants are defined and distinct for each signal.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.signal.h.023
 *
 * Only <signal.h> is included (pc.h brings in <stddef.h> alone), so a
 * type or name this test uses that the header does not provide stops
 * the build, and the test is UNINITIATED.
 */
#include <signal.h>
#include "pc.h"

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

/* Fails unless the n values in v are all different. */
static void distinct(const char *what, const long *v, const char *const *name, int n)
{
	int i, j;

	for (i = 0; i < n; i++)
		for (j = i + 1; j < n; j++)
			pc_check(v[i] != v[j], "%s: %s and %s are both %ld", what, name[i], name[j], v[i]);
}

int main(void)
{
	pc_begin("XBD.signal.h.023");
	pc_test();
#if !defined(ILL_ILLOPC) || !defined(ILL_ILLOPN) || !defined(ILL_ILLADR) || !defined(ILL_ILLTRP) || !defined(ILL_PRVOPC) || !defined(ILL_PRVREG) || !defined(ILL_COPROC) || !defined(ILL_BADSTK) || !defined(FPE_INTDIV) || !defined(FPE_INTOVF) || !defined(FPE_FLTDIV) || !defined(FPE_FLTOVF) || !defined(FPE_FLTUND) || !defined(FPE_FLTRES) || !defined(FPE_FLTINV) || !defined(FPE_FLTSUB) || !defined(SEGV_MAPERR) || !defined(SEGV_ACCERR) || !defined(BUS_ADRALN) || !defined(BUS_ADRERR) || !defined(BUS_OBJERR) || !defined(CLD_EXITED) || !defined(CLD_KILLED) || !defined(CLD_DUMPED) || !defined(CLD_TRAPPED) || !defined(CLD_STOPPED) || !defined(CLD_CONTINUED) || !defined(SI_USER) || !defined(SI_QUEUE) || !defined(SI_TIMER) || !defined(SI_ASYNCIO) || !defined(SI_MESGQ)
	pc_fail("<signal.h> does not define every si_code constant: missing one of ILL_ILLOPC ILL_ILLOPN ILL_ILLADR ILL_ILLTRP ILL_PRVOPC ILL_PRVREG ILL_COPROC ILL_BADSTK FPE_INTDIV FPE_INTOVF FPE_FLTDIV FPE_FLTOVF FPE_FLTUND FPE_FLTRES FPE_FLTINV FPE_FLTSUB SEGV_MAPERR SEGV_ACCERR BUS_ADRALN BUS_ADRERR BUS_OBJERR CLD_EXITED CLD_KILLED CLD_DUMPED CLD_TRAPPED CLD_STOPPED CLD_CONTINUED SI_USER SI_QUEUE SI_TIMER SI_ASYNCIO SI_MESGQ");
#else
	{
		_Static_assert((ILL_ILLOPC) == (ILL_ILLOPC), "ILL_ILLOPC is an integer constant expression");
		_Static_assert((ILL_ILLOPN) == (ILL_ILLOPN), "ILL_ILLOPN is an integer constant expression");
		_Static_assert((ILL_ILLADR) == (ILL_ILLADR), "ILL_ILLADR is an integer constant expression");
		_Static_assert((ILL_ILLTRP) == (ILL_ILLTRP), "ILL_ILLTRP is an integer constant expression");
		_Static_assert((ILL_PRVOPC) == (ILL_PRVOPC), "ILL_PRVOPC is an integer constant expression");
		_Static_assert((ILL_PRVREG) == (ILL_PRVREG), "ILL_PRVREG is an integer constant expression");
		_Static_assert((ILL_COPROC) == (ILL_COPROC), "ILL_COPROC is an integer constant expression");
		_Static_assert((ILL_BADSTK) == (ILL_BADSTK), "ILL_BADSTK is an integer constant expression");
#define E_ILL_ILLOPC (ILL_ILLOPC)
#define E_ILL_ILLOPN (ILL_ILLOPN)
#define E_ILL_ILLADR (ILL_ILLADR)
#define E_ILL_ILLTRP (ILL_ILLTRP)
#define E_ILL_PRVOPC (ILL_PRVOPC)
#define E_ILL_PRVREG (ILL_PRVREG)
#define E_ILL_COPROC (ILL_COPROC)
#define E_ILL_BADSTK (ILL_BADSTK)
		static const long v[] = { E_ILL_ILLOPC, E_ILL_ILLOPN, E_ILL_ILLADR, E_ILL_ILLTRP, E_ILL_PRVOPC, E_ILL_PRVREG, E_ILL_COPROC, E_ILL_BADSTK };
		static const char *const name[] = { "ILL_ILLOPC", "ILL_ILLOPN", "ILL_ILLADR", "ILL_ILLTRP", "ILL_PRVOPC", "ILL_PRVREG", "ILL_COPROC", "ILL_BADSTK" };
		distinct("si_code values for SIGILL", v, name, 8);
	}
	{
		_Static_assert((FPE_INTDIV) == (FPE_INTDIV), "FPE_INTDIV is an integer constant expression");
		_Static_assert((FPE_INTOVF) == (FPE_INTOVF), "FPE_INTOVF is an integer constant expression");
		_Static_assert((FPE_FLTDIV) == (FPE_FLTDIV), "FPE_FLTDIV is an integer constant expression");
		_Static_assert((FPE_FLTOVF) == (FPE_FLTOVF), "FPE_FLTOVF is an integer constant expression");
		_Static_assert((FPE_FLTUND) == (FPE_FLTUND), "FPE_FLTUND is an integer constant expression");
		_Static_assert((FPE_FLTRES) == (FPE_FLTRES), "FPE_FLTRES is an integer constant expression");
		_Static_assert((FPE_FLTINV) == (FPE_FLTINV), "FPE_FLTINV is an integer constant expression");
		_Static_assert((FPE_FLTSUB) == (FPE_FLTSUB), "FPE_FLTSUB is an integer constant expression");
#define E_FPE_INTDIV (FPE_INTDIV)
#define E_FPE_INTOVF (FPE_INTOVF)
#define E_FPE_FLTDIV (FPE_FLTDIV)
#define E_FPE_FLTOVF (FPE_FLTOVF)
#define E_FPE_FLTUND (FPE_FLTUND)
#define E_FPE_FLTRES (FPE_FLTRES)
#define E_FPE_FLTINV (FPE_FLTINV)
#define E_FPE_FLTSUB (FPE_FLTSUB)
		static const long v[] = { E_FPE_INTDIV, E_FPE_INTOVF, E_FPE_FLTDIV, E_FPE_FLTOVF, E_FPE_FLTUND, E_FPE_FLTRES, E_FPE_FLTINV, E_FPE_FLTSUB };
		static const char *const name[] = { "FPE_INTDIV", "FPE_INTOVF", "FPE_FLTDIV", "FPE_FLTOVF", "FPE_FLTUND", "FPE_FLTRES", "FPE_FLTINV", "FPE_FLTSUB" };
		distinct("si_code values for SIGFPE", v, name, 8);
	}
	{
		_Static_assert((SEGV_MAPERR) == (SEGV_MAPERR), "SEGV_MAPERR is an integer constant expression");
		_Static_assert((SEGV_ACCERR) == (SEGV_ACCERR), "SEGV_ACCERR is an integer constant expression");
#define E_SEGV_MAPERR (SEGV_MAPERR)
#define E_SEGV_ACCERR (SEGV_ACCERR)
		static const long v[] = { E_SEGV_MAPERR, E_SEGV_ACCERR };
		static const char *const name[] = { "SEGV_MAPERR", "SEGV_ACCERR" };
		distinct("si_code values for SIGSEGV", v, name, 2);
	}
	{
		_Static_assert((BUS_ADRALN) == (BUS_ADRALN), "BUS_ADRALN is an integer constant expression");
		_Static_assert((BUS_ADRERR) == (BUS_ADRERR), "BUS_ADRERR is an integer constant expression");
		_Static_assert((BUS_OBJERR) == (BUS_OBJERR), "BUS_OBJERR is an integer constant expression");
#define E_BUS_ADRALN (BUS_ADRALN)
#define E_BUS_ADRERR (BUS_ADRERR)
#define E_BUS_OBJERR (BUS_OBJERR)
		static const long v[] = { E_BUS_ADRALN, E_BUS_ADRERR, E_BUS_OBJERR };
		static const char *const name[] = { "BUS_ADRALN", "BUS_ADRERR", "BUS_OBJERR" };
		distinct("si_code values for SIGBUS", v, name, 3);
	}
	{
		_Static_assert((CLD_EXITED) == (CLD_EXITED), "CLD_EXITED is an integer constant expression");
		_Static_assert((CLD_KILLED) == (CLD_KILLED), "CLD_KILLED is an integer constant expression");
		_Static_assert((CLD_DUMPED) == (CLD_DUMPED), "CLD_DUMPED is an integer constant expression");
		_Static_assert((CLD_TRAPPED) == (CLD_TRAPPED), "CLD_TRAPPED is an integer constant expression");
		_Static_assert((CLD_STOPPED) == (CLD_STOPPED), "CLD_STOPPED is an integer constant expression");
		_Static_assert((CLD_CONTINUED) == (CLD_CONTINUED), "CLD_CONTINUED is an integer constant expression");
#define E_CLD_EXITED (CLD_EXITED)
#define E_CLD_KILLED (CLD_KILLED)
#define E_CLD_DUMPED (CLD_DUMPED)
#define E_CLD_TRAPPED (CLD_TRAPPED)
#define E_CLD_STOPPED (CLD_STOPPED)
#define E_CLD_CONTINUED (CLD_CONTINUED)
		static const long v[] = { E_CLD_EXITED, E_CLD_KILLED, E_CLD_DUMPED, E_CLD_TRAPPED, E_CLD_STOPPED, E_CLD_CONTINUED };
		static const char *const name[] = { "CLD_EXITED", "CLD_KILLED", "CLD_DUMPED", "CLD_TRAPPED", "CLD_STOPPED", "CLD_CONTINUED" };
		distinct("si_code values for SIGCHLD", v, name, 6);
	}
	{
		_Static_assert((SI_USER) == (SI_USER), "SI_USER is an integer constant expression");
		_Static_assert((SI_QUEUE) == (SI_QUEUE), "SI_QUEUE is an integer constant expression");
		_Static_assert((SI_TIMER) == (SI_TIMER), "SI_TIMER is an integer constant expression");
		_Static_assert((SI_ASYNCIO) == (SI_ASYNCIO), "SI_ASYNCIO is an integer constant expression");
		_Static_assert((SI_MESGQ) == (SI_MESGQ), "SI_MESGQ is an integer constant expression");
#define E_SI_USER (SI_USER)
#define E_SI_QUEUE (SI_QUEUE)
#define E_SI_TIMER (SI_TIMER)
#define E_SI_ASYNCIO (SI_ASYNCIO)
#define E_SI_MESGQ (SI_MESGQ)
		static const long v[] = { E_SI_USER, E_SI_QUEUE, E_SI_TIMER, E_SI_ASYNCIO, E_SI_MESGQ };
		static const char *const name[] = { "SI_USER", "SI_QUEUE", "SI_TIMER", "SI_ASYNCIO", "SI_MESGQ" };
		distinct("si_code values for any signal", v, name, 5);
	}
	pc_pass();
#endif
}
