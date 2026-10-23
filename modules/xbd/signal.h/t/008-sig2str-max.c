/*
 * SIG2STR_MAX is defined, usable in #if, and big enough for every required name.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.signal.h.008
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

#include <string.h>

int main(void)
{
	pc_begin("XBD.signal.h.008");
	pc_test();
#if !defined(SIG2STR_MAX)
	pc_fail("<signal.h> does not define SIG2STR_MAX");
#elif SIG2STR_MAX < 1
	pc_fail("SIG2STR_MAX is %d", (int)SIG2STR_MAX);
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
		int i, n = 0, digits = 1;
		size_t need;

		for (i = 0; i < nn; i++)
			pc_check(strlen(named_n[i]) - 3 + 1 <= (size_t)SIG2STR_MAX, "SIG2STR_MAX (%d) is too small for %s", (int)SIG2STR_MAX, named_n[i] + 3);
#if defined(SIGRTMIN) && defined(SIGRTMAX)	/* their absence is XBD.signal.h.006's FAIL */
		n = (SIGRTMIN + SIGRTMAX) / 2 - SIGRTMIN;
#endif
		while (n >= 10) {
			n /= 10;
			digits++;
		}
		need = strlen("RTMIN+") + (size_t)digits + 1;
		pc_check(need <= (size_t)SIG2STR_MAX, "SIG2STR_MAX (%d) is below the %zu bytes of the longest RTMIN+n name", (int)SIG2STR_MAX, need);
	}
	pc_pass();
#endif
}
