/*
 * setpgid.c - planted faults in setpgid(), for mutation checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "setpgid.<fault>" and apply only while
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
	size_t n = strlen("setpgid.");

	if (ph == NULL || strcmp(ph, "test") != 0 || m == NULL || strncmp(m, "setpgid.", n) != 0)
		return NULL;
	return m + n;
}

#define IS(name) (m != NULL && strcmp(m, name) == 0)
#define REAL(fn) static __typeof__(fn) *real_##fn; \
	if (real_##fn == NULL) real_##fn = (__typeof__(fn) *)dlsym(RTLD_NEXT, #fn)

static int fail(int e)
{
	errno = e;
	return -1;
}

int setpgid(pid_t pid, pid_t pgid)
{
	REAL(setpgid);
	const char *m = mutant();
	pid_t target = pid == 0 ? getpid() : pid;
	int r, e;

	if (IS("noop"))
		return 0;
	if (IS("caller"))
		return real_setpgid(0, pgid);
	if (IS("pid0_esrch") && pid == 0)
		return fail(ESRCH);
	if (IS("pid0_parent") && pid == 0)
		pid = getppid();
	if (IS("pgid0_caller") && pgid == 0)
		pgid = getpid();
	if (IS("new_group"))
		pgid = 0;
	if (IS("neg_ok") && pgid < 0)
		return 0;
	r = real_setpgid(pid, pgid);
	if (r == 0)
		return 0;
	e = errno;
	if (e == EPERM) {
		int leader = getsid(target) == target;
		int other = getsid(target) != getsid(0);

		if (IS("eperm_esrch"))
			return fail(ESRCH);
		if (IS("leader_ok") && leader)
			return 0;
		if (IS("session_ok") && !leader && other)
			return 0;
		if (IS("nogroup_ok") && !leader && !other)
			return 0;
	}
	if (e == EACCES) {
		if (IS("eacces_eperm"))
			return fail(EPERM);
		if (IS("exec_ok"))
			return 0;
	}
	if (e == EINVAL && IS("einval_eperm"))
		return fail(EPERM);
	if (e == ESRCH) {
		if (IS("esrch_eperm"))
			return fail(EPERM);
		if (IS("esrch_ok"))
			return 0;
	}
	return fail(e);
}
