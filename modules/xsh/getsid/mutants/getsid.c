/*
 * getsid.c - planted faults in getsid(), for mutation checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "getsid.<fault>" and apply only while
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
	size_t n = strlen("getsid.");

	if (ph == NULL || strcmp(ph, "test") != 0 || m == NULL || strncmp(m, "getsid.", n) != 0)
		return NULL;
	return m + n;
}

#define IS(name) (m != NULL && strcmp(m, name) == 0)
#define REAL(fn) static __typeof__(fn) *real_##fn; \
	if (real_##fn == NULL) real_##fn = (__typeof__(fn) *)dlsym(RTLD_NEXT, #fn)

pid_t getsid(pid_t pid)
{
	REAL(getsid);
	const char *m = mutant();
	pid_t r;

	if (IS("pgrp"))		/* the group, not the session */
		return getpgid(pid);
	if (IS("self"))
		return real_getsid(0);
	if (IS("zero_esrch") && pid == 0) {
		errno = ESRCH;
		return -1;
	}
	if (IS("zero_pgrp") && pid == 0)
		return getpgrp();
	r = real_getsid(pid);
	if (r == -1 && errno == ESRCH) {
		if (IS("esrch_eperm")) {
			errno = EPERM;
			return -1;
		}
		if (IS("esrch_self"))
			return real_getsid(0);
	}
	return r;
}
