/*
 * setsid.c - planted faults in setsid(), for mutation checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "setsid.<fault>" and apply only while
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
	size_t n = strlen("setsid.");

	if (ph == NULL || strcmp(ph, "test") != 0 || m == NULL || strncmp(m, "setsid.", n) != 0)
		return NULL;
	return m + n;
}

#define IS(name) (m != NULL && strcmp(m, name) == 0)
#define REAL(fn) static __typeof__(fn) *real_##fn; \
	if (real_##fn == NULL) real_##fn = (__typeof__(fn) *)dlsym(RTLD_NEXT, #fn)

pid_t setsid(void)
{
	REAL(setsid);
	const char *m = mutant();
	pid_t r;

	if (IS("group_only")) {	/* a new group, no new session */
		if (setpgid(0, 0) != 0)
			return -1;
		return getpid();
	}
	if (IS("leader_ok") && getpgrp() == getpid())
		return getpid();
	r = real_setsid();
	if (r == -1 && errno == EPERM) {
		if (IS("leader_einval") && getpgrp() == getpid())
			errno = EINVAL;
		if (IS("only_leader_check") && getpgrp() != getpid())
			return getpid();
		if (IS("in_use_esrch") && getpgrp() != getpid())
			errno = ESRCH;
		return -1;
	}
	if (IS("ret_zero") && r != -1)
		return 0;
	return r;
}
