/*
 * mlock.c - planted faults in mlock() and munlock(), for mutation checks
 * (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "mlock.<fault>" and apply only
 * while PC_PHASE is "test".
 */
#define _GNU_SOURCE 1
#include <dlfcn.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

static const char *mutant(void)
{
	const char *ph = getenv("PC_PHASE");
	const char *m = getenv("PC_MUTANT");

	if (ph == NULL || strcmp(ph, "test") != 0 || m == NULL || strncmp(m, "mlock.", 6) != 0)
		return NULL;
	return m + 6;
}

#define IS(name) (m != NULL && strcmp(m, name) == 0)
#define REAL(fn) static __typeof__(fn) *real_##fn; \
	if (real_##fn == NULL) real_##fn = (__typeof__(fn) *)dlsym(RTLD_NEXT, #fn)

/* Ranges locked in the test phase, with how many times each was locked. */
static struct rec {
	const void *addr;
	size_t len;
	int count;
} recs[32];
static int nrecs;

static struct rec *find(const void *addr, size_t len)
{
	int i;

	for (i = 0; i < nrecs; i++)
		if (recs[i].addr == addr && recs[i].len == len)
			return &recs[i];
	if (nrecs == 32)
		return NULL;
	recs[nrecs].addr = addr;
	recs[nrecs].len = len;
	recs[nrecs].count = 0;
	return &recs[nrecs++];
}

int mlock(const void *addr, size_t len)
{
	REAL(mlock);
	const char *m = mutant();
	struct rec *r;
	int ret;

	if (IS("noop"))
		return 0;
	ret = real_mlock(addr, len);
	if (ret == 0 && (r = find(addr, len)) != NULL)
		r->count++;
	if (IS("returns_one") && ret == 0)
		return 1;
	if (IS("unmapped_ok") && ret == -1 && errno == ENOMEM)
		return 0;
	if (IS("partial") && ret == -1 && errno == ENOMEM) {
		(void)real_mlock(addr, (size_t)sysconf(_SC_PAGESIZE));	/* the part before the hole stays locked */
		errno = ENOMEM;
	}
	return ret;
}

int munlock(const void *addr, size_t len)
{
	REAL(munlock);
	const char *m = mutant();
	struct rec *r;
	int ret, i;

	if (IS("munlock_noop"))
		return 0;
	if (IS("counted") && (r = find(addr, len)) != NULL && --r->count > 0)
		return 0;	/* waits for as many munlock() calls as mlock() calls */
	ret = real_munlock(addr, len);
	if (IS("unmapped_ok") && ret == -1 && errno == ENOMEM)
		return 0;
	if (IS("partial") && ret == -1 && errno == ENOMEM) {
		(void)real_munlock(addr, (size_t)sysconf(_SC_PAGESIZE));
		errno = ENOMEM;
	}
	if (IS("munlock_all") && ret == 0)
		for (i = 0; i < nrecs; i++)
			(void)real_munlock(recs[i].addr, recs[i].len);	/* unlocks every lock on the pages */
	return ret;
}
