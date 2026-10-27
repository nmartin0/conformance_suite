/*
 * aio_cancel.c - planted faults in the asynchronous I/O interface (aio_cancel), for
 * mutation checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "aio_cancel.<fault>" and apply only while
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

#define PFX "aio_cancel."

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

int aio_cancel(int fd, struct aiocb *cb)
{
	const char *m = mutant();
	int r;
	REAL(aio_cancel);

	if (IS("cancel_noop") && cb != NULL)
		return AIO_CANCELED;
	if (IS("null_noop") && cb == NULL)
		return AIO_CANCELED;
	if (IS("null_alldone") && cb == NULL)
		return AIO_ALLDONE;
	if (IS("badfd_alldone") && fd < 0)
		return AIO_ALLDONE;
	r = real_aio_cancel(fd, cb);
	if (IS("notcanceled_for_done") && r == AIO_ALLDONE)
		return AIO_NOTCANCELED;
	if (IS("canceled_for_done") && r == AIO_ALLDONE)
		return AIO_CANCELED;
	if (IS("lies_alldone") && (r == AIO_CANCELED || r == AIO_NOTCANCELED))
		return AIO_ALLDONE;
	if (IS("ebadf_as_einval") && r == -1 && errno == EBADF)
		errno = EINVAL;
	return r;
}
