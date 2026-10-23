/*
 * psiginfo.c - planted faults in psignal() and psiginfo(), for mutation checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "psiginfo.<fault>" and apply only while
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

/* Writes message and description the faulty way the fault asks for. */
static void faulty(const char *m, int sig, const char *msg)
{
	const char *d = strsignal(sig);
	char buf[512];
	int n;

	if (IS("silent"))
		return;
	if (IS("orients_wide"))
		(void)fwide(stderr, 1);
	if (msg != NULL && (*msg != '\0' || IS("prefix_always")))
		n = snprintf(buf, sizeof buf, IS("no_separator") ? "%s%s%s" : "%s: %s%s", msg, d, IS("no_newline") ? "" : "\n");
	else if (IS("prefix_always"))
		n = snprintf(buf, sizeof buf, ": %s\n", d);
	else
		n = snprintf(buf, sizeof buf, "%s%s", d, IS("no_newline") ? "" : "\n");
	if (IS("orients_wide"))
		(void)fwprintf(stderr, L"%s", buf);
	else if (IS("error_hidden"))
		(void)write(2, buf, (size_t)n);	/* the error is not reported */
	else
		(void)fputs(buf, stderr);
	if (IS("errno_clobbered"))
		errno = ENOENT;
}

static int handles(const char *m)
{
	return IS("silent") || IS("no_separator") || IS("no_newline") || IS("prefix_always") ||
	    IS("orients_wide") || IS("errno_clobbered") || IS("error_hidden");
}

void psignal(int sig, const char *msg)
{
	REAL(psignal);
	const char *m = mutant_of("psiginfo.");
	int e = errno;

	if (m == NULL || !handles(m)) {
		real_psignal(sig, msg);
		return;
	}
	faulty(m, sig, msg);
	if (!IS("errno_clobbered") && !IS("error_hidden"))
		errno = e;
}

void psiginfo(const siginfo_t *si, const char *msg)
{
	REAL(psiginfo);
	const char *m = mutant_of("psiginfo.");
	int e = errno;

	if (m == NULL) {
		real_psiginfo(si, msg);
		return;
	}
	if (IS("info_wrong_signal")) {
		((__typeof__(psignal) *)dlsym(RTLD_NEXT, "psignal"))(si->si_signo == SIGUSR1 ? SIGUSR2 : SIGUSR1, msg);
		return;
	}
	if (!handles(m)) {
		real_psiginfo(si, msg);
		return;
	}
	faulty(m, si->si_signo, msg);
	if (!IS("errno_clobbered") && !IS("error_hidden"))
		errno = e;
}
