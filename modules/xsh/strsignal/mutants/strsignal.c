/*
 * strsignal.c - planted faults in strsignal() (and psignal() and
 * psiginfo() where they share its buffer), for mutation checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "strsignal.<fault>" and apply only
 * while PC_PHASE is "test" (as set by pc.c in this process or a parent).
 */
#define _GNU_SOURCE 1
#include <dlfcn.h>
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char phase[16];

static size_t my_len(const char *s)
{
	size_t n = 0;

	while (s[n] != '\0')
		n++;
	return n;
}

static int my_eq(const char *a, const char *b)
{
	while (*a != '\0' && *a == *b)
		a++, b++;
	return *a == *b;
}

/* pc.c sets PC_PHASE with setenv(); remember it in this process. */
int setenv(const char *name, const char *value, int overwrite)
{
	static int (*real)(const char *, const char *, int);

	if (real == NULL)
		real = (int (*)(const char *, const char *, int))dlsym(RTLD_NEXT, "setenv");
	if (my_eq(name, "PC_PHASE") && my_len(value) < sizeof phase)
		memcpy(phase, value, my_len(value) + 1);
	return real(name, value, overwrite);
}

static const char *mutant_of(const char *prefix)
{
	const char *m = getenv("PC_MUTANT");
	size_t n = my_len(prefix);

	if (!my_eq(phase, "test") || m == NULL || strncmp(m, prefix, n) != 0)
		return NULL;
	return m + n;
}

#define IS(name) (m != NULL && my_eq(m, name))
#define REAL(fn) static __typeof__(fn) *real_##fn; \
	if (real_##fn == NULL) real_##fn = (__typeof__(fn) *)dlsym(RTLD_NEXT, #fn)

static char buf[256];

char *strsignal(int sig)
{
	REAL(strsignal);
	const char *m = mutant_of("strsignal.");
	char *s = real_strsignal(sig);

	if (m == NULL)
		return s;
	if (IS("text_differs")) {
		/* Its own wording, unlike what psignal() writes. */
		(void)snprintf(buf, sizeof buf, "Signal %d", sig);
		return buf;
	}
	if (IS("null_valid") && sig == SIGUSR1)
		return NULL;
	if (IS("null_rt") && sig >= SIGRTMIN && sig <= SIGRTMAX)
		return NULL;
	if (IS("psignal_overwrites") || IS("psiginfo_overwrites")) {
		(void)snprintf(buf, sizeof buf, "%s", s != NULL ? s : "?");
		return buf;
	}
	return s;
}

/* The description written through strsignal() and its one buffer. */
static void through_strsignal(int sig, const char *msg)
{
	const char *d = strsignal(sig);

	if (msg != NULL && *msg != '\0')
		fprintf(stderr, "%s: %s\n", msg, d);
	else
		fprintf(stderr, "%s\n", d);
}

void psignal(int sig, const char *msg)
{
	REAL(psignal);
	const char *m = mutant_of("strsignal.");
	int e = errno;

	if (IS("psignal_overwrites")) {
		through_strsignal(sig, msg);
		errno = e;
		return;
	}
	real_psignal(sig, msg);
}

void psiginfo(const siginfo_t *si, const char *msg)
{
	REAL(psiginfo);
	const char *m = mutant_of("strsignal.");
	int e = errno;

	if (IS("psiginfo_overwrites")) {
		through_strsignal(si->si_signo, msg);
		errno = e;
		return;
	}
	real_psiginfo(si, msg);
}
