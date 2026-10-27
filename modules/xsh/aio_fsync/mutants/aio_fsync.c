/*
 * aio_fsync.c - planted faults in the asynchronous I/O interface (aio_fsync), for
 * mutation checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "aio_fsync.<fault>" and apply only while
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

#define PFX "aio_fsync."

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

int aio_fsync(int op, struct aiocb *cb)
{
	const char *m = mutant();
	int r;
	REAL(aio_fsync);

	if (IS("rejects_op")) {
		errno = EINVAL;
		return -1;
	}
	if (IS("junk_member_einval") && cb->aio_nbytes != 0) {
		errno = EINVAL;
		return -1;
	}
	if (IS("badfd_ok") && cb->aio_fildes < 0)
		return 0;
	if (IS("bad_op_accepted") && op != O_SYNC && op != O_DSYNC)
		op = O_SYNC;
	r = real_aio_fsync(op, cb);
	if (IS("ebadf_as_einval") && r == -1 && errno == EBADF)
		errno = EINVAL;
	return r;
}
