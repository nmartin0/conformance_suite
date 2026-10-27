/*
 * aio_return.c - planted faults in the asynchronous I/O interface (aio_return), for
 * mutation checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "aio_return.<fault>" and apply only while
 * PC_PHASE is "test".
 */
#define _GNU_SOURCE 1
#include <aio.h>
#include <dlfcn.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define PFX "aio_return."

static const char *mutant(void)
{
	const char *ph = getenv("PC_PHASE");
	const char *m = getenv("PC_MUTANT");

	if (ph == NULL || strcmp(ph, "test") != 0 || m == NULL ||
	    strncmp(m, PFX, strlen(PFX)) != 0)
		return NULL;
	return m + strlen(PFX);
}

#define IS(name) (m != NULL && strcmp(m, name) == 0)
#define REAL(fn) static __typeof__(fn) *real_##fn; \
	if (real_##fn == NULL) real_##fn = (__typeof__(fn) *)dlsym(RTLD_NEXT, #fn)

ssize_t aio_return(struct aiocb *cb)
{
	static const struct aiocb *last;
	static ssize_t lastv;
	const char *m = mutant();
	ssize_t r;
	REAL(aio_return);

	r = real_aio_return(cb);
	if (IS("stale") && cb == last)
		return lastv;
	last = cb;
	lastv = r;
	if (IS("off_by_one") && r > 0)
		return r - 1;
	if (IS("always_zero") && r > 0)
		return 0;
	if (IS("failure_hidden") && r == -1)
		return 0;
	return r;
}
