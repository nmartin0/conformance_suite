/*
 * sig2str.c - planted faults in sig2str() and str2sig(), for mutation checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "sig2str.<fault>" and apply only while
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

/*
 * Where the C library lacks these functions, the shim then has
 * nothing to wrap, and the tests do not build there anyway.
 */
int sig2str(int signum, char *str)
{
	int (*real)(int, char *) = (int (*)(int, char *))dlsym(RTLD_NEXT, "sig2str");
	const char *m = mutant_of("sig2str.");
	int r;

	if (real == NULL)
		return -1;
	if (m == NULL)
		return real(signum, str);
	if (IS("accepts_invalid") && (signum < 0 || signum > 64)) {
		strcpy(str, "BOGUS");
		return 0;
	}
	if (IS("rt_off_by_one") && signum > SIGRTMIN && signum < SIGRTMAX)
		signum++;
	r = real(signum, str);
	if (r == 0 && IS("keeps_prefix")) {
		char t[64];
		(void)snprintf(t, sizeof t, "SIG%s", str);
		strcpy(str, t);
	}
	if (r == 0 && IS("rt_padded") && strncmp(str, "RTMIN+", 6) == 0 && str[7] == '\0') {
		str[8] = '\0';
		str[7] = str[6];
		str[6] = '0';
	}
	if (r == 0 && IS("returns_one"))
		return 1;
	return r;
}

int str2sig(const char *restrict str, int *restrict pnum)
{
	int (*real)(const char *, int *) = (int (*)(const char *, int *))dlsym(RTLD_NEXT, "str2sig");
	const char *m = mutant_of("sig2str.");
	int r;

	if (real == NULL)
		return -1;
	if (m == NULL)
		return real(str, pnum);
	if (IS("str2sig_wants_prefix") && strncmp(str, "SIG", 3) != 0)
		return -1;
	if (IS("str2sig_no_numbers") && str[0] >= '0' && str[0] <= '9')
		return -1;
	r = real(str, pnum);
	if (r == 0 && IS("str2sig_rt_off_by_one") && (strncmp(str, "RTMIN+", 6) == 0 || strncmp(str, "RTMAX-", 6) == 0))
		(*pnum)++;
	if (r == -1 && IS("str2sig_stores_on_failure"))
		*pnum = 0;
	return r;
}
