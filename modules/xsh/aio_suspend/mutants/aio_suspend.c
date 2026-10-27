/*
 * aio_suspend.c - planted faults in the asynchronous I/O interface (aio_suspend), for
 * mutation checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "aio_suspend.<fault>" and apply only while
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

#define PFX "aio_suspend."

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

int aio_suspend(const struct aiocb *const list[], int nent, const struct timespec *timeout)
{
	const char *m = mutant();
	struct timespec t;
	int r, i;
	REAL(aio_suspend);

	if (IS("returns_early") && timeout != NULL) {
		t.tv_sec = timeout->tv_sec / 3;
		t.tv_nsec = timeout->tv_nsec / 3;
		timeout = &t;
	}
	if (IS("only_first") && nent > 1)
		nent = 1;
	if (IS("zero_timeout_eagain") && timeout != NULL &&
	    timeout->tv_sec == 0 && timeout->tv_nsec == 0) {
		errno = EAGAIN;
		return -1;
	}
	for (i = 0; i < nent; i++) {
		if (list[i] != NULL)
			continue;
		if (IS("null_is_done"))
			return 0;
		if (IS("null_einval")) {
			errno = EINVAL;
			return -1;
		}
	}
	if (IS("returns_at_once") && timeout == NULL)
		return 0;
	r = real_aio_suspend(list, nent, timeout);
	if (IS("wrong_errno") && r == -1 && errno == EAGAIN)
		errno = ETIMEDOUT;
	if (IS("returns_zero") && r == -1 && errno == EAGAIN)
		return 0;
	if (IS("fails_after_wake") && r == 0) {
		errno = EINTR;
		return -1;
	}
	if (IS("eintr_as_eagain") && r == -1 && errno == EINTR)
		errno = EAGAIN;
	if (IS("eintr_as_zero") && r == -1 && errno == EINTR)
		return 0;
	return r;
}
