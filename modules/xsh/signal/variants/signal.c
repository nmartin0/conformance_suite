/*
 * signal.c - conforming variants of signal(), for the variant checks (CI
 * only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; see core/pcvariant.h. Each variant says why the
 * standard allows it.
 */
#define _GNU_SOURCE 1
#include <errno.h>
#include <signal.h>
#include <string.h>
#include "pcvariant.h"

/*
 * "block_all": while a handler set with signal() runs, every signal that
 * can be blocked is held off, the action is not reset, and interrupted
 * functions are not restarted. XSH signal() lets the implementation either
 * reset the action or hold off an implementation-defined set of signals
 * that includes the one caught; it says nothing about restarting, so an
 * interrupted function fails as its page describes (XSH 2.4.4). glibc
 * (in strict POSIX mode) resets; musl holds off only the signal and
 * restarts.
 */
static void (*set(const char *name, int sig, void (*fn)(int)))(int)
{
	void (*(*real)(int, void (*)(int)))(int);
	const char *v = variant_of("signal.");
	struct sigaction a, o;

	real = (void (*(*)(int, void (*)(int)))(int))dlsym(RTLD_NEXT, name);
	if (!IS("block_all"))
		return real == NULL ? SIG_ERR : real(sig, fn);
	memset(&a, 0, sizeof a);
	a.sa_handler = fn;
	a.sa_flags = 0;
	if (fn == SIG_DFL || fn == SIG_IGN)
		sigemptyset(&a.sa_mask);
	else
		sigfillset(&a.sa_mask);
	if (fn == SIG_ERR) {
		errno = EINVAL;
		return SIG_ERR;
	}
	if (sigaction(sig, &a, &o) != 0)
		return SIG_ERR;	/* errno from sigaction(): EINVAL */
	if (o.sa_flags & SA_SIGINFO)
		return (void (*)(int))(void (*)(void))o.sa_sigaction;
	return o.sa_handler;
}

void (*signal(int sig, void (*fn)(int)))(int)
{
	return set("signal", sig, fn);
}

/* glibc maps signal() to one of these, depending on the feature macros */
void (*__sysv_signal(int sig, void (*fn)(int)))(int)
{
	return set("__sysv_signal", sig, fn);
}

void (*bsd_signal(int sig, void (*fn)(int)))(int)
{
	return set("bsd_signal", sig, fn);
}
