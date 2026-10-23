/*
 * killpg.c - planted faults in killpg(), for mutation checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "killpg.<fault>" and apply only while
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

int killpg(pid_t pgrp, int sig)
{
	REAL(killpg);
	const char *m = mutant_of("killpg.");

	if (m == NULL)
		return real_killpg(pgrp, sig);
	if (IS("esrch_hidden") || IS("eperm_hidden")) {
		int r = real_killpg(pgrp, sig);
		if (r == -1 && ((IS("esrch_hidden") && errno == ESRCH) || (IS("eperm_hidden") && errno == EPERM)))
			return 0;
		return r;
	}
	if (IS("leader_only"))
		return kill(pgrp, sig);
	if (IS("drops") && sig != 0 && sig != SIGKILL && pgrp != getpgrp())
		return 0;
	if (IS("null_sends") && sig == 0)
		sig = SIGUSR1;
	if (IS("einval_accepted") && (sig < 0 || sig > 64))
		return 0;
	return real_killpg(pgrp, sig);
}
