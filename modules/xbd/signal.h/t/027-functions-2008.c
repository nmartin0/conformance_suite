/*
 * pthread_kill(), pthread_sigmask(), sigqueue(), sigtimedwait(), sigwaitinfo(), psiginfo() and psignal() are declared.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.signal.h.027
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
	pc_begin("XBD.signal.h.027");
	pc_test();
	pc_check(IS_T(&(pthread_kill), int (*)(pthread_t, int)), "pthread_kill() is not declared as int (*)(pthread_t, int)");
	pc_check(!IS_T(&(pthread_kill), int (*)(pthread_t, int, int)), "pthread_kill() is declared without a prototype");
	pc_check(IS_T(&(pthread_sigmask), int (*)(int, const sigset_t *, sigset_t *)), "pthread_sigmask() is not declared as int (*)(int, const sigset_t *, sigset_t *)");
	pc_check(!IS_T(&(pthread_sigmask), int (*)(int, const sigset_t *, sigset_t *, int)), "pthread_sigmask() is declared without a prototype");
	pc_check(IS_T(&(sigqueue), int (*)(pid_t, int, union sigval)), "sigqueue() is not declared as int (*)(pid_t, int, union sigval)");
	pc_check(!IS_T(&(sigqueue), int (*)(pid_t, int, union sigval, int)), "sigqueue() is declared without a prototype");
	pc_check(IS_T(&(sigtimedwait), int (*)(const sigset_t *, siginfo_t *, const struct timespec *)), "sigtimedwait() is not declared as int (*)(const sigset_t *, siginfo_t *, const struct timespec *)");
	pc_check(!IS_T(&(sigtimedwait), int (*)(const sigset_t *, siginfo_t *, const struct timespec *, int)), "sigtimedwait() is declared without a prototype");
	pc_check(IS_T(&(sigwaitinfo), int (*)(const sigset_t *, siginfo_t *)), "sigwaitinfo() is not declared as int (*)(const sigset_t *, siginfo_t *)");
	pc_check(!IS_T(&(sigwaitinfo), int (*)(const sigset_t *, siginfo_t *, int)), "sigwaitinfo() is declared without a prototype");
	pc_check(IS_T(&(psiginfo), void (*)(const siginfo_t *, const char *)), "psiginfo() is not declared as void (*)(const siginfo_t *, const char *)");
	pc_check(!IS_T(&(psiginfo), void (*)(const siginfo_t *, const char *, int)), "psiginfo() is declared without a prototype");
	pc_check(IS_T(&(psignal), void (*)(int, const char *)), "psignal() is not declared as void (*)(int, const char *)");
	pc_check(!IS_T(&(psignal), void (*)(int, const char *, int)), "psignal() is declared without a prototype");
	pc_pass();
}
