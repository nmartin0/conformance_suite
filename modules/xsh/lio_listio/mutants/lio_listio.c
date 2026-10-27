/*
 * lio_listio.c - planted faults in the asynchronous I/O interface (lio_listio), for
 * mutation checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "lio_listio.<fault>" and apply only while
 * PC_PHASE is "test".
 */
#define _GNU_SOURCE 1
/* The header's own declaration is renamed away: its qualifiers differ between systems. */
#define lio_listio lio_listio_declared
#include <aio.h>
#undef lio_listio
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

#define PFX "lio_listio."

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

/* See ci/mutate.sh: a fault that cannot be planted here is UNCHECKED. */
static void cannot_plant(const char *m)
{
	fprintf(stderr, "pcmutant: cannot plant " PFX "%s here\n", m);
}

int lio_listio(int mode, struct aiocb *restrict const list[restrict], int nent,
    struct sigevent *restrict sig)
{
	static struct sigevent copy;
	const char *m = mutant();
	int r, i;
	REAL(lio_listio);

	if (IS("wait_as_nowait") && mode == LIO_WAIT) {
		mode = LIO_NOWAIT;
		sig = NULL;
	}
	if (IS("nowait_blocks") && mode == LIO_NOWAIT)
		mode = LIO_WAIT;
	if (IS("bad_mode_runs") && mode != LIO_WAIT && mode != LIO_NOWAIT)
		mode = LIO_WAIT;
	if (IS("nop_as_read"))
		for (i = 0; i < nent; i++)
			if (list[i] != NULL && list[i]->aio_lio_opcode == LIO_NOP)
				list[i]->aio_lio_opcode = LIO_READ;
	if (IS("null_stops"))
		for (i = 0; i < nent; i++)
			if (list[i] == NULL) {
				nent = i;
				break;
			}
	if (IS("stops_at_failure"))
		for (i = 0; i < nent; i++)
			if (list[i] != NULL && list[i]->aio_lio_opcode == LIO_READ &&
			    (fcntl(list[i]->aio_fildes, F_GETFL) & O_ACCMODE) == O_WRONLY) {
				nent = i + 1;
				break;
			}
	if (IS("no_limit_check")) {
		long mx = sysconf(_SC_AIO_LISTIO_MAX);

		if (mx > 0 && nent > mx)
			nent = (int)mx;
		else if (mx <= 0)
			cannot_plant(m);
	}
	if (IS("value_lost") && sig != NULL) {
		copy = *sig;
		copy.sigev_value.sival_ptr = NULL;
		sig = &copy;
	}
	if (IS("no_notify"))
		sig = NULL;
	if (IS("notify_early") && mode == LIO_NOWAIT && sig != NULL &&
	    sig->sigev_notify == SIGEV_SIGNAL) {
		struct sigevent ev = *sig;

		r = real_lio_listio(mode, list, nent, NULL);
		if (r == 0)
			(void)sigqueue(getpid(), ev.sigev_signo, ev.sigev_value);
		return r;
	}
	r = real_lio_listio(mode, list, nent, sig);
	if (IS("sig_not_ignored") && mode == LIO_WAIT && sig != NULL &&
	    sig->sigev_notify == SIGEV_SIGNAL)
		(void)kill(getpid(), sig->sigev_signo);
	if (IS("eio_as_eagain") && r == -1 && errno == EIO)
		errno = EAGAIN;
	if (IS("ok_despite_failure") && r == -1 && errno == EIO)
		return 0;
	return r;
}
