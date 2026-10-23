/*
 * signal.c - planted faults in signal(), for mutation checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "signal.<fault>" and apply only while
 * PC_PHASE is "test" (as set by pc.c in this process or a parent).
 */
#define _GNU_SOURCE 1
#include <dlfcn.h>
#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wchar.h>

static char phase[16];

/* pc.c sets PC_PHASE with setenv(); remember it in this process. */
int setenv(const char *name, const char *value, int overwrite)
{
	static int (*real)(const char *, const char *, int);

	if (real == NULL)
		real = (int (*)(const char *, const char *, int))dlsym(RTLD_NEXT, "setenv");
	if (strcmp(name, "PC_PHASE") == 0 && strlen(value) < sizeof phase)
		strcpy(phase, value);
	return real(name, value, overwrite);
}

static const char *mutant_of(const char *prefix)
{
	const char *m = getenv("PC_MUTANT");
	size_t n = strlen(prefix);

	if (strcmp(phase, "test") != 0 || m == NULL || strncmp(m, prefix, n) != 0)
		return NULL;
	return m + n;
}

#define IS(name) (m != NULL && strcmp(m, name) == 0)
#define REAL(fn) static __typeof__(fn) *real_##fn; \
	if (real_##fn == NULL) real_##fn = (__typeof__(fn) *)dlsym(RTLD_NEXT, #fn)

static void (*prev[65])(int);
static void (*user[65])(int);

static void tramp(int sig)
{
	user[sig](0);
}

static void (*fake(const char *name, int sig, void (*fn)(int)))(int)
{
	void (*(*real)(int, void (*)(int)))(int);
	const char *m = mutant_of("signal.");
	void (*r)(int);
	struct sigaction a, o;

	real = (void (*(*)(int, void (*)(int)))(int))dlsym(RTLD_NEXT, name);
	if (m == NULL || real == NULL)
		return real == NULL ? SIG_ERR : real(sig, fn);
	if (IS("no_einval") && (sig <= 0 || sig > 64 || sig == SIGKILL || sig == SIGSTOP))
		return SIG_DFL;
	if (IS("ign_as_dfl") && fn == SIG_IGN)
		fn = SIG_DFL;
	if (IS("dfl_ignored") && fn == SIG_DFL)
		return SIG_DFL;
	if (IS("wrong_signo") && fn != SIG_DFL && fn != SIG_IGN && sig > 0 && sig < 65) {
		/* the handler is called through a trampoline that passes 0 */
		user[sig] = fn;
		fn = tramp;
	}
	if (IS("neither_reset_nor_block") && fn != SIG_DFL && fn != SIG_IGN) {
		memset(&a, 0, sizeof a);
		a.sa_handler = fn;
		a.sa_flags = SA_NODEFER;
		sigemptyset(&a.sa_mask);
		if (sigaction(sig, &a, &o) != 0)
			return SIG_ERR;
		return o.sa_handler;
	}
	r = real(sig, fn);
	if (IS("returns_new") && r != SIG_ERR)
		return fn;
	if (IS("returns_dfl") && r != SIG_ERR)
		return SIG_DFL;
	if (IS("errno_clobbered") && r != SIG_ERR)
		errno = ENOENT;
	(void)prev;
	return r;
}

void (*signal(int sig, void (*fn)(int)))(int)
{
	return fake("signal", sig, fn);
}

/* glibc maps signal() to one of these, depending on the feature macros */
void (*__sysv_signal(int sig, void (*fn)(int)))(int)
{
	return fake("__sysv_signal", sig, fn);
}

void (*bsd_signal(int sig, void (*fn)(int)))(int)
{
	return fake("bsd_signal", sig, fn);
}
