/*
 * close.c - conforming variants of close() and posix_close(), for the
 * variant checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; see core/pcvariant.h. Each variant says why the
 * standard allows it.
 */
#define _GNU_SOURCE 1
#include <aio.h>
#include <errno.h>
#include <fcntl.h>
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>
#include "pcvariant.h"

/*
 * "aio_cancel" and "aio_wait": close() DESCRIPTION leaves it to the
 * implementation whether, and which, outstanding asynchronous operations
 * on the descriptor are canceled; those not canceled complete as if
 * close() had blocked until they did, and close() need not block (so it
 * may). aio_cancel cancels what it can and then waits for the rest;
 * aio_wait cancels nothing and waits for every one. To know what is
 * outstanding, this library notes the control blocks passed to
 * aio_read() and aio_write().
 */
#define NCB 64
static struct aiocb *volatile cbs[NCB];

static void note_cb(struct aiocb *cb)
{
	int i;

	for (i = 0; i < NCB; i++)
		if (cbs[i] == cb)
			return;
	for (i = 0; i < NCB; i++)
		if (__sync_bool_compare_and_swap(&cbs[i], NULL, cb))
			return;
}

int aio_write(struct aiocb *cb)
{
	const char *v = variant_of("close.");
	REAL(aio_write);

	if (IS("aio_cancel") || IS("aio_wait"))
		note_cb(cb);
	return real_aio_write(cb);
}

int aio_read(struct aiocb *cb)
{
	const char *v = variant_of("close.");
	REAL(aio_read);

	if (IS("aio_cancel") || IS("aio_wait"))
		note_cb(cb);
	return real_aio_read(cb);
}

static void settle_aio(const char *v, int fd)
{
	int i, any = 0;

	for (i = 0; i < NCB; i++)
		if (cbs[i] != NULL && cbs[i]->aio_fildes == fd)
			any = 1;
	if (!any)
		return;
	if (IS("aio_cancel")) {
#ifndef __GLIBC__
		/*
		 * musl's aio_cancel() never returns here: the same fault as its
		 * close() (docs/linux-deviations.md O.30), so the variant rests
		 * on the code under suspicion.
		 */
		fprintf(stderr, "pcmutant: cannot plant close.aio_cancel here "
		    "(this aio_cancel() is the failing code, O.30)\n");
#endif
		(void)aio_cancel(fd, NULL);
	}
	for (i = 0; i < NCB; i++) {
		struct aiocb *cb = cbs[i];
		const struct aiocb *one[1];

		if (cb == NULL || cb->aio_fildes != fd)
			continue;
		one[0] = cb;
		while (aio_error(cb) == EINPROGRESS)
			(void)aio_suspend(one, 1, NULL);
		cbs[i] = NULL;
	}
}

int close(int fd)
{
	const char *v = variant_of("close.");
	REAL(close);

	if (IS("aio_cancel") || IS("aio_wait"))
		settle_aio(v, fd);
	return real_close(fd);
}

#ifdef POSIX_CLOSE_RESTART
/*
 * "pc_einval": posix_close() with an invalid flag may fail with EINVAL but
 * otherwise behaves as with flag 0 and closes the descriptor (close()
 * DESCRIPTION and ERRORS); this system closes it and reports EINVAL.
 *
 * "restart_bit": POSIX_CLOSE_RESTART is non-zero (header half
 * close.restart_bit.h); <unistd.h> allows 0 or another value. The
 * restart behaviour only matters when a signal interrupts the call; here
 * the bit is taken off and the call made as with that bit alone.
 */
#define V_RESTART 0x40000000

int posix_close(int fd, int flag)
{
	const char *v = variant_of("close.");
	REAL(posix_close);

	if (IS("restart_bit") && (flag & V_RESTART))
		flag = (flag & ~V_RESTART) | POSIX_CLOSE_RESTART;
	if (IS("pc_einval") && flag != 0 && flag != POSIX_CLOSE_RESTART) {
		(void)real_posix_close(fd, 0);
		errno = EINVAL;
		return -1;
	}
	return real_posix_close(fd, flag);
}
#endif

/*
 * "hidden_fds": the implementation keeps descriptors of its own open in
 * the process (close() APPLICATION USAGE: catalogs, tracing); their
 * numbers are not specified, and XSH 2.6 still allocates the lowest one
 * not open. One sits just above the lowest free descriptor at start-up
 * (leaving a gap below it), one at 300; both close-on-exec, so each new
 * program gets its own.
 */
__attribute__((constructor)) static void hidden_fds(void)
{
	const char *v = variant_of("close.");
	int n;

	if (!IS("hidden_fds"))
		return;
	n = open("/dev/null", O_RDONLY | O_CLOEXEC);
	if (n < 0)
		return;
	(void)fcntl(n, F_DUPFD_CLOEXEC, n + 1);
	(void)fcntl(n, F_DUPFD_CLOEXEC, 300);
	(void)close(n);
}
