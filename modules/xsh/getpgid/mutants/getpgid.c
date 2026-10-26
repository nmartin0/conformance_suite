/*
 * getpgid.c - planted faults in getpgid(), for mutation checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "getpgid.<fault>" and apply only while
 * PC_PHASE is "test".
 */
#define _GNU_SOURCE 1
#include <dlfcn.h>
#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static const char *mutant(void)
{
	const char *ph = getenv("PC_PHASE");
	const char *m = getenv("PC_MUTANT");
	size_t n = strlen("getpgid.");

	if (ph == NULL || strcmp(ph, "test") != 0 || m == NULL || strncmp(m, "getpgid.", n) != 0)
		return NULL;
	return m + n;
}

#define IS(name) (m != NULL && strcmp(m, name) == 0)
#define REAL(fn) static __typeof__(fn) *real_##fn; \
	if (real_##fn == NULL) real_##fn = (__typeof__(fn) *)dlsym(RTLD_NEXT, #fn)

pid_t getpgid(pid_t pid)
{
	REAL(getpgid);
	const char *m = mutant();
	pid_t r;

	if (IS("self"))		/* ignores pid */
		return real_getpgid(0);
	if (IS("pid") && pid > 0)
		return pid;
	if (IS("zero_esrch") && pid == 0) {
		errno = ESRCH;
		return -1;
	}
	if (IS("zero_pid") && pid == 0)
		return getpid();
	if (IS("neg_self") && pid < 0)
		return real_getpgid(0);
	if (IS("neg_eperm") && pid < 0) {
		errno = EPERM;
		return -1;
	}
	if (IS("hide_esrch") && pid > 0 && getsid(pid) != getsid(0)) {
		errno = ESRCH;	/* hides processes of other sessions with the wrong error */
		return -1;
	}
	r = real_getpgid(pid);
	if (r == -1 && errno == ESRCH) {
		if (IS("esrch_einval")) {
			errno = EINVAL;
			return -1;
		}
		if (IS("esrch_self"))
			return real_getpgid(0);
	}
	return r;
}
