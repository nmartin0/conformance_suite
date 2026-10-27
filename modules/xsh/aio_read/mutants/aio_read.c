/*
 * aio_read.c - planted faults in the asynchronous I/O interface (aio_read), for
 * mutation checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "aio_read.<fault>" and apply only while
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

#define PFX "aio_read."

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

int aio_read(struct aiocb *cb)
{
	const char *m = mutant();
	int r;
	REAL(aio_read);

	if (IS("offset_ignored"))
		cb->aio_offset = 0;
	if (IS("uses_fd_offset")) {
		off_t o = lseek(cb->aio_fildes, 0, SEEK_CUR);
		if (o >= 0)
			cb->aio_offset = o;
	}
	if (IS("nbytes_short") && cb->aio_nbytes > 1)
		cb->aio_nbytes--;
	if (IS("zero_reads_one") && cb->aio_nbytes == 0)
		cb->aio_nbytes = 1;
	if (IS("honours_opcode")) {
		/* lio_listio() semantics where aio_read() must ignore them. */
		if (cb->aio_lio_opcode == LIO_WRITE)
			return aio_write(cb);
		if (cb->aio_lio_opcode == LIO_NOP)
			return 0;
	}
	if (IS("badfd_queued") && cb->aio_fildes < 0)
		return 0;
	if (IS("negative_ok") && cb->aio_offset < 0)
		cb->aio_offset = 0;
	if (IS("reqprio_ignored"))
		cb->aio_reqprio = 0;
	if (IS("value_lost"))
		cb->aio_sigevent.sigev_value.sival_ptr = NULL;
	if (IS("no_signal"))
		cb->aio_sigevent.sigev_notify = SIGEV_NONE;
	r = real_aio_read(cb);
	if (IS("ebadf_as_einval") && r == -1 && errno == EBADF)
		errno = EINVAL;
	if (IS("blocks") && r == 0)
		while (aio_error(cb) == EINPROGRESS)
			usleep(10000);
	return r;
}
