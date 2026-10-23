/*
 * Signal numbers are below sysconf(_SC_NSIG), which is at most {NSIG_MAX}.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.signal.h.007
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

#include <limits.h>
#include <unistd.h>
#ifdef _SC_NSIG
#define defined_sc_nsig 1
#else
#define defined_sc_nsig 0
#endif

int main(void)
{
	pc_begin("XBD.signal.h.007");
	pc_test();
#if !defined(_SC_NSIG) || !defined(NSIG_MAX)
	pc_fail("%s", !defined_sc_nsig ? "<unistd.h> does not define _SC_NSIG" : "<limits.h> does not define NSIG_MAX");
#else
	{
		int named[27], nn = 0;
		const char *named_n[27];
#ifdef SIGABRT
		named_n[nn] = "SIGABRT";
		named[nn++] = SIGABRT;
#endif
#ifdef SIGFPE
		named_n[nn] = "SIGFPE";
		named[nn++] = SIGFPE;
#endif
#ifdef SIGILL
		named_n[nn] = "SIGILL";
		named[nn++] = SIGILL;
#endif
#ifdef SIGINT
		named_n[nn] = "SIGINT";
		named[nn++] = SIGINT;
#endif
#ifdef SIGSEGV
		named_n[nn] = "SIGSEGV";
		named[nn++] = SIGSEGV;
#endif
#ifdef SIGTERM
		named_n[nn] = "SIGTERM";
		named[nn++] = SIGTERM;
#endif
#ifdef SIGALRM
		named_n[nn] = "SIGALRM";
		named[nn++] = SIGALRM;
#endif
#ifdef SIGHUP
		named_n[nn] = "SIGHUP";
		named[nn++] = SIGHUP;
#endif
#ifdef SIGKILL
		named_n[nn] = "SIGKILL";
		named[nn++] = SIGKILL;
#endif
#ifdef SIGPIPE
		named_n[nn] = "SIGPIPE";
		named[nn++] = SIGPIPE;
#endif
#ifdef SIGQUIT
		named_n[nn] = "SIGQUIT";
		named[nn++] = SIGQUIT;
#endif
#ifdef SIGUSR1
		named_n[nn] = "SIGUSR1";
		named[nn++] = SIGUSR1;
#endif
#ifdef SIGUSR2
		named_n[nn] = "SIGUSR2";
		named[nn++] = SIGUSR2;
#endif
#ifdef SIGCHLD
		named_n[nn] = "SIGCHLD";
		named[nn++] = SIGCHLD;
#endif
#ifdef SIGCONT
		named_n[nn] = "SIGCONT";
		named[nn++] = SIGCONT;
#endif
#ifdef SIGSTOP
		named_n[nn] = "SIGSTOP";
		named[nn++] = SIGSTOP;
#endif
#ifdef SIGTSTP
		named_n[nn] = "SIGTSTP";
		named[nn++] = SIGTSTP;
#endif
#ifdef SIGTTIN
		named_n[nn] = "SIGTTIN";
		named[nn++] = SIGTTIN;
#endif
#ifdef SIGTTOU
		named_n[nn] = "SIGTTOU";
		named[nn++] = SIGTTOU;
#endif
#ifdef SIGBUS
		named_n[nn] = "SIGBUS";
		named[nn++] = SIGBUS;
#endif
#ifdef SIGURG
		named_n[nn] = "SIGURG";
		named[nn++] = SIGURG;
#endif
#ifdef SIGWINCH
		named_n[nn] = "SIGWINCH";
		named[nn++] = SIGWINCH;
#endif
#ifdef SIGSYS
		named_n[nn] = "SIGSYS";
		named[nn++] = SIGSYS;
#endif
#ifdef SIGTRAP
		named_n[nn] = "SIGTRAP";
		named[nn++] = SIGTRAP;
#endif
#ifdef SIGVTALRM
		named_n[nn] = "SIGVTALRM";
		named[nn++] = SIGVTALRM;
#endif
#ifdef SIGXCPU
		named_n[nn] = "SIGXCPU";
		named[nn++] = SIGXCPU;
#endif
#ifdef SIGXFSZ
		named_n[nn] = "SIGXFSZ";
		named[nn++] = SIGXFSZ;
#endif
		long ns = sysconf(_SC_NSIG);
		int i;

		pc_check(ns > 0, "sysconf(_SC_NSIG) gave %ld", ns);
		pc_check(ns <= (long)NSIG_MAX, "sysconf(_SC_NSIG) (%ld) is above {NSIG_MAX} (%ld)", ns, (long)NSIG_MAX);
#ifdef SIGRTMAX	/* its absence is XBD.signal.h.006's FAIL */
		pc_check(SIGRTMAX < ns, "SIGRTMAX (%d) is not below sysconf(_SC_NSIG) (%ld)", SIGRTMAX, ns);
#endif
		for (i = 0; i < nn; i++)
			pc_check(named[i] < (long)NSIG_MAX, "%s (%d) is not below {NSIG_MAX} (%ld)", named_n[i], named[i], (long)NSIG_MAX);
	}
	pc_pass();
#endif
}
