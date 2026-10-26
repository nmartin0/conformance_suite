/*
 * getgroups.c - planted faults in getgroups(), for mutation checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "getgroups.<fault>" and apply only while
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
	size_t n = strlen("getgroups.");

	if (ph == NULL || strcmp(ph, "test") != 0 || m == NULL || strncmp(m, "getgroups.", n) != 0)
		return NULL;
	return m + n;
}

#define IS(name) (m != NULL && strcmp(m, name) == 0)
#define REAL(fn) static __typeof__(fn) *real_##fn; \
	if (real_##fn == NULL) real_##fn = (__typeof__(fn) *)dlsym(RTLD_NEXT, #fn)

int getgroups(int size, gid_t list[])
{
	REAL(getgroups);
	const char *m = mutant();
	int r;

	if (IS("empty"))
		return 0;
	if (IS("egid_only")) {
		if (size >= 1)
			list[0] = getegid();
		return 1;
	}
	if (IS("zero_returns_zero") && size == 0)
		return 0;
	if (IS("zero_writes") && size == 0) {
		gid_t tmp[256];

		r = real_getgroups(256, tmp);
		if (r > 0)
			memcpy(list, tmp, (size_t)r * sizeof *tmp);
		return r;
	}
	if (size < 0) {
		if (IS("neg_ok"))
			return real_getgroups(0, list);
		if (IS("neg_efault")) {
			errno = EFAULT;
			return -1;
		}
	}
	if (IS("short_ok") && size > 0) {
		gid_t tmp[256];

		r = real_getgroups(256, tmp);
		if (r > size) {
			memcpy(list, tmp, (size_t)size * sizeof *tmp);
			return size;
		}
	}
	r = real_getgroups(size, list);
	if (IS("off_by_one") && r > 0 && size > 0)
		return r - 1;
	return r;
}
