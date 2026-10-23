/*
 * The XSI signals are defined and distinct from the others.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.signal.h.012
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
	pc_begin("XBD.signal.h.012");
	pc_test();
#if !defined(SIGABRT) || !defined(SIGFPE) || !defined(SIGILL) || !defined(SIGINT) || !defined(SIGSEGV) || !defined(SIGTERM) || !defined(SIGALRM) || !defined(SIGHUP) || !defined(SIGKILL) || !defined(SIGPIPE) || !defined(SIGQUIT) || !defined(SIGUSR1) || !defined(SIGUSR2) || !defined(SIGCHLD) || !defined(SIGCONT) || !defined(SIGSTOP) || !defined(SIGTSTP) || !defined(SIGTTIN) || !defined(SIGTTOU) || !defined(SIGBUS) || !defined(SIGURG) || !defined(SIGWINCH) || !defined(SIGSYS) || !defined(SIGTRAP) || !defined(SIGVTALRM) || !defined(SIGXCPU) || !defined(SIGXFSZ)
	pc_fail("<signal.h> does not define every one of: SIGABRT SIGFPE SIGILL SIGINT SIGSEGV SIGTERM SIGALRM SIGHUP SIGKILL SIGPIPE SIGQUIT SIGUSR1 SIGUSR2 SIGCHLD SIGCONT SIGSTOP SIGTSTP SIGTTIN SIGTTOU SIGBUS SIGURG SIGWINCH SIGSYS SIGTRAP SIGVTALRM SIGXCPU SIGXFSZ");
#else
	{
		enum { E_SIGABRT = SIGABRT, E_SIGFPE = SIGFPE, E_SIGILL = SIGILL, E_SIGINT = SIGINT, E_SIGSEGV = SIGSEGV, E_SIGTERM = SIGTERM, E_SIGALRM = SIGALRM, E_SIGHUP = SIGHUP, E_SIGKILL = SIGKILL, E_SIGPIPE = SIGPIPE, E_SIGQUIT = SIGQUIT, E_SIGUSR1 = SIGUSR1, E_SIGUSR2 = SIGUSR2, E_SIGCHLD = SIGCHLD, E_SIGCONT = SIGCONT, E_SIGSTOP = SIGSTOP, E_SIGTSTP = SIGTSTP, E_SIGTTIN = SIGTTIN, E_SIGTTOU = SIGTTOU, E_SIGBUS = SIGBUS, E_SIGURG = SIGURG, E_SIGWINCH = SIGWINCH, E_SIGSYS = SIGSYS, E_SIGTRAP = SIGTRAP, E_SIGVTALRM = SIGVTALRM, E_SIGXCPU = SIGXCPU, E_SIGXFSZ = SIGXFSZ };	/* integer constant expressions */
		static const long v[] = { SIGABRT, SIGFPE, SIGILL, SIGINT, SIGSEGV, SIGTERM, SIGALRM, SIGHUP, SIGKILL, SIGPIPE, SIGQUIT, SIGUSR1, SIGUSR2, SIGCHLD, SIGCONT, SIGSTOP, SIGTSTP, SIGTTIN, SIGTTOU, SIGBUS, SIGURG, SIGWINCH, SIGSYS, SIGTRAP, SIGVTALRM, SIGXCPU, SIGXFSZ };
		static const char *const name[] = { "SIGABRT", "SIGFPE", "SIGILL", "SIGINT", "SIGSEGV", "SIGTERM", "SIGALRM", "SIGHUP", "SIGKILL", "SIGPIPE", "SIGQUIT", "SIGUSR1", "SIGUSR2", "SIGCHLD", "SIGCONT", "SIGSTOP", "SIGTSTP", "SIGTTIN", "SIGTTOU", "SIGBUS", "SIGURG", "SIGWINCH", "SIGSYS", "SIGTRAP", "SIGVTALRM", "SIGXCPU", "SIGXFSZ" };
		int i, n = (int)(sizeof v / sizeof v[0]);
		int ints = IS_T(SIGABRT, int) && IS_T(SIGFPE, int) && IS_T(SIGILL, int) && IS_T(SIGINT, int) && IS_T(SIGSEGV, int) && IS_T(SIGTERM, int) && IS_T(SIGALRM, int) && IS_T(SIGHUP, int) && IS_T(SIGKILL, int) && IS_T(SIGPIPE, int) && IS_T(SIGQUIT, int) && IS_T(SIGUSR1, int) && IS_T(SIGUSR2, int) && IS_T(SIGCHLD, int) && IS_T(SIGCONT, int) && IS_T(SIGSTOP, int) && IS_T(SIGTSTP, int) && IS_T(SIGTTIN, int) && IS_T(SIGTTOU, int) && IS_T(SIGBUS, int) && IS_T(SIGURG, int) && IS_T(SIGWINCH, int) && IS_T(SIGSYS, int) && IS_T(SIGTRAP, int) && IS_T(SIGVTALRM, int) && IS_T(SIGXCPU, int) && IS_T(SIGXFSZ, int);

		pc_check(ints, "a signal macro does not have type int");
		for (i = 0; i < n; i++)
			pc_check(v[i] > 0, "%s is %ld, not positive", name[i], v[i]);
		distinct("signal", v, name, n);
	}
	pc_pass();
#endif
}
