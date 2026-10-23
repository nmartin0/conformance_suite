/*
 * The original signal functions are declared with prototypes.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.signal.h.025
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
	pc_begin("XBD.signal.h.025");
	pc_test();
	pc_check(IS_T(&(kill), int (*)(pid_t, int)), "kill() is not declared as int (*)(pid_t, int)");
	pc_check(!IS_T(&(kill), int (*)(pid_t, int, int)), "kill() is declared without a prototype");
	pc_check(IS_T(&(raise), int (*)(int)), "raise() is not declared as int (*)(int)");
	pc_check(!IS_T(&(raise), int (*)(int, int)), "raise() is declared without a prototype");
	pc_check(IS_T(&(sigaction), int (*)(int, const struct sigaction *, struct sigaction *)), "sigaction() is not declared as int (*)(int, const struct sigaction *, struct sigaction *)");
	pc_check(!IS_T(&(sigaction), int (*)(int, const struct sigaction *, struct sigaction *, int)), "sigaction() is declared without a prototype");
	pc_check(IS_T(&(sigaddset), int (*)(sigset_t *, int)), "sigaddset() is not declared as int (*)(sigset_t *, int)");
	pc_check(!IS_T(&(sigaddset), int (*)(sigset_t *, int, int)), "sigaddset() is declared without a prototype");
	pc_check(IS_T(&(sigdelset), int (*)(sigset_t *, int)), "sigdelset() is not declared as int (*)(sigset_t *, int)");
	pc_check(!IS_T(&(sigdelset), int (*)(sigset_t *, int, int)), "sigdelset() is declared without a prototype");
	pc_check(IS_T(&(sigemptyset), int (*)(sigset_t *)), "sigemptyset() is not declared as int (*)(sigset_t *)");
	pc_check(!IS_T(&(sigemptyset), int (*)(sigset_t *, int)), "sigemptyset() is declared without a prototype");
	pc_check(IS_T(&(sigfillset), int (*)(sigset_t *)), "sigfillset() is not declared as int (*)(sigset_t *)");
	pc_check(!IS_T(&(sigfillset), int (*)(sigset_t *, int)), "sigfillset() is declared without a prototype");
	pc_check(IS_T(&(sigismember), int (*)(const sigset_t *, int)), "sigismember() is not declared as int (*)(const sigset_t *, int)");
	pc_check(!IS_T(&(sigismember), int (*)(const sigset_t *, int, int)), "sigismember() is declared without a prototype");
	pc_check(IS_T(&(signal), void (*(*)(int, void (*)(int)))(int)), "signal() is not declared as void (*(*)(int, void (*)(int)))(int)");
	pc_check(!IS_T(&(signal), void (*(*)(int, void (*)(int), int))(int)), "signal() is declared without a prototype");
	pc_check(IS_T(&(sigpending), int (*)(sigset_t *)), "sigpending() is not declared as int (*)(sigset_t *)");
	pc_check(!IS_T(&(sigpending), int (*)(sigset_t *, int)), "sigpending() is declared without a prototype");
	pc_check(IS_T(&(sigprocmask), int (*)(int, const sigset_t *, sigset_t *)), "sigprocmask() is not declared as int (*)(int, const sigset_t *, sigset_t *)");
	pc_check(!IS_T(&(sigprocmask), int (*)(int, const sigset_t *, sigset_t *, int)), "sigprocmask() is declared without a prototype");
	pc_check(IS_T(&(sigsuspend), int (*)(const sigset_t *)), "sigsuspend() is not declared as int (*)(const sigset_t *)");
	pc_check(!IS_T(&(sigsuspend), int (*)(const sigset_t *, int)), "sigsuspend() is declared without a prototype");
	pc_pass();
}
