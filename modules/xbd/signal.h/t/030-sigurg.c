/*
 * SIGURG is defined and distinct from the others.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.signal.h.030
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
	pc_begin("XBD.signal.h.030");
	pc_test();
#if !defined(SIGABRT) || !defined(SIGFPE) || !defined(SIGILL) || !defined(SIGINT) || !defined(SIGSEGV) || !defined(SIGTERM) || !defined(SIGALRM) || !defined(SIGHUP) || !defined(SIGKILL) || !defined(SIGPIPE) || !defined(SIGQUIT) || !defined(SIGUSR1) || !defined(SIGUSR2) || !defined(SIGCHLD) || !defined(SIGCONT) || !defined(SIGSTOP) || !defined(SIGTSTP) || !defined(SIGTTIN) || !defined(SIGTTOU) || !defined(SIGBUS) || !defined(SIGURG)
	pc_fail("<signal.h> does not define every one of: SIGABRT SIGFPE SIGILL SIGINT SIGSEGV SIGTERM SIGALRM SIGHUP SIGKILL SIGPIPE SIGQUIT SIGUSR1 SIGUSR2 SIGCHLD SIGCONT SIGSTOP SIGTSTP SIGTTIN SIGTTOU SIGBUS SIGURG");
#else
	{
		_Static_assert((SIGABRT) == (SIGABRT), "SIGABRT is an integer constant expression");
		_Static_assert((SIGFPE) == (SIGFPE), "SIGFPE is an integer constant expression");
		_Static_assert((SIGILL) == (SIGILL), "SIGILL is an integer constant expression");
		_Static_assert((SIGINT) == (SIGINT), "SIGINT is an integer constant expression");
		_Static_assert((SIGSEGV) == (SIGSEGV), "SIGSEGV is an integer constant expression");
		_Static_assert((SIGTERM) == (SIGTERM), "SIGTERM is an integer constant expression");
		_Static_assert((SIGALRM) == (SIGALRM), "SIGALRM is an integer constant expression");
		_Static_assert((SIGHUP) == (SIGHUP), "SIGHUP is an integer constant expression");
		_Static_assert((SIGKILL) == (SIGKILL), "SIGKILL is an integer constant expression");
		_Static_assert((SIGPIPE) == (SIGPIPE), "SIGPIPE is an integer constant expression");
		_Static_assert((SIGQUIT) == (SIGQUIT), "SIGQUIT is an integer constant expression");
		_Static_assert((SIGUSR1) == (SIGUSR1), "SIGUSR1 is an integer constant expression");
		_Static_assert((SIGUSR2) == (SIGUSR2), "SIGUSR2 is an integer constant expression");
		_Static_assert((SIGCHLD) == (SIGCHLD), "SIGCHLD is an integer constant expression");
		_Static_assert((SIGCONT) == (SIGCONT), "SIGCONT is an integer constant expression");
		_Static_assert((SIGSTOP) == (SIGSTOP), "SIGSTOP is an integer constant expression");
		_Static_assert((SIGTSTP) == (SIGTSTP), "SIGTSTP is an integer constant expression");
		_Static_assert((SIGTTIN) == (SIGTTIN), "SIGTTIN is an integer constant expression");
		_Static_assert((SIGTTOU) == (SIGTTOU), "SIGTTOU is an integer constant expression");
		_Static_assert((SIGBUS) == (SIGBUS), "SIGBUS is an integer constant expression");
		_Static_assert((SIGURG) == (SIGURG), "SIGURG is an integer constant expression");
#define E_SIGABRT (SIGABRT)
#define E_SIGFPE (SIGFPE)
#define E_SIGILL (SIGILL)
#define E_SIGINT (SIGINT)
#define E_SIGSEGV (SIGSEGV)
#define E_SIGTERM (SIGTERM)
#define E_SIGALRM (SIGALRM)
#define E_SIGHUP (SIGHUP)
#define E_SIGKILL (SIGKILL)
#define E_SIGPIPE (SIGPIPE)
#define E_SIGQUIT (SIGQUIT)
#define E_SIGUSR1 (SIGUSR1)
#define E_SIGUSR2 (SIGUSR2)
#define E_SIGCHLD (SIGCHLD)
#define E_SIGCONT (SIGCONT)
#define E_SIGSTOP (SIGSTOP)
#define E_SIGTSTP (SIGTSTP)
#define E_SIGTTIN (SIGTTIN)
#define E_SIGTTOU (SIGTTOU)
#define E_SIGBUS (SIGBUS)
#define E_SIGURG (SIGURG)
		static const long v[] = { SIGABRT, SIGFPE, SIGILL, SIGINT, SIGSEGV, SIGTERM, SIGALRM, SIGHUP, SIGKILL, SIGPIPE, SIGQUIT, SIGUSR1, SIGUSR2, SIGCHLD, SIGCONT, SIGSTOP, SIGTSTP, SIGTTIN, SIGTTOU, SIGBUS, SIGURG };
		static const char *const name[] = { "SIGABRT", "SIGFPE", "SIGILL", "SIGINT", "SIGSEGV", "SIGTERM", "SIGALRM", "SIGHUP", "SIGKILL", "SIGPIPE", "SIGQUIT", "SIGUSR1", "SIGUSR2", "SIGCHLD", "SIGCONT", "SIGSTOP", "SIGTSTP", "SIGTTIN", "SIGTTOU", "SIGBUS", "SIGURG" };
		int i, n = (int)(sizeof v / sizeof v[0]);
		int ints = IS_T(SIGABRT, int) && IS_T(SIGFPE, int) && IS_T(SIGILL, int) && IS_T(SIGINT, int) && IS_T(SIGSEGV, int) && IS_T(SIGTERM, int) && IS_T(SIGALRM, int) && IS_T(SIGHUP, int) && IS_T(SIGKILL, int) && IS_T(SIGPIPE, int) && IS_T(SIGQUIT, int) && IS_T(SIGUSR1, int) && IS_T(SIGUSR2, int) && IS_T(SIGCHLD, int) && IS_T(SIGCONT, int) && IS_T(SIGSTOP, int) && IS_T(SIGTSTP, int) && IS_T(SIGTTIN, int) && IS_T(SIGTTOU, int) && IS_T(SIGBUS, int) && IS_T(SIGURG, int);

		pc_check(ints, "a signal macro does not have type int");
		for (i = 0; i < n; i++)
			pc_check(v[i] > 0, "%s is %ld, not positive", name[i], v[i]);
		distinct("signal", v, name, n);
	}
	pc_pass();
#endif
}
