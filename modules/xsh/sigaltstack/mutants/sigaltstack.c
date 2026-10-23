/*
 * sigaltstack.c - planted faults in sigaltstack(), for mutation checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "sigaltstack.<fault>" and apply only while
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

int sigaltstack(const stack_t *ss, stack_t *oss)
{
	REAL(sigaltstack);
	const char *m = mutant_of("sigaltstack.");
	stack_t n;
	int r;

	if (m == NULL)
		return real_sigaltstack(ss, oss);
	if (IS("ss_ignored") && ss != NULL)
		ss = NULL;
	if (IS("disable_ignored") && ss != NULL && (ss->ss_flags & SS_DISABLE))
		ss = NULL;
	if (IS("small_ok") && ss != NULL && ss->ss_size < (size_t)MINSIGSTKSZ) {
		n = *ss;
		n.ss_size = (size_t)MINSIGSTKSZ;
		ss = &n;
	}
	if (IS("flags_accepted") && ss != NULL && !(ss->ss_flags & SS_DISABLE)) {
		n = *ss;
		n.ss_flags = 0;
		ss = &n;
	}
	r = real_sigaltstack(ss, oss);
	if (r == -1 && IS("eperm_hidden") && errno == EPERM)
		r = 0;
	if (r == 0 && oss != NULL && IS("oss_zeroed"))
		memset(oss, 0, sizeof *oss);
	if (r == 0 && oss != NULL && IS("oss_flags_lost"))
		oss->ss_flags = 0;
	return r;
}
