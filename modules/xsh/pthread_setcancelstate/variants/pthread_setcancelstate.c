/*
 * pthread_setcancelstate.c - conforming variants of the cancelability
 * type, for the variant checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; see core/pcvariant.h. Each variant says why the
 * standard allows it.
 *
 * "async_deferred": a thread that sets PTHREAD_CANCEL_ASYNCHRONOUS is
 * told so (the type is kept, and reported as the previous type), but
 * requests are still acted on only at cancellation points. XSH 2.9.5.1
 * says that with the asynchronous type requests "may" be acted upon at
 * any time, which can be read as a permission and not a duty; under
 * that reading this system conforms (XSH.pthread_setcancelstate.006 is
 * disputed). The type is per thread, and every new thread starts
 * DEFERRED.
 *
 * Acting on a request resets the type to DEFERRED (XSH 2.9.5.3). The
 * real type here is always DEFERRED, so a request is acted on only inside
 * a cancellation point; the cancellation points below are wrapped so
 * that the reported type is DEFERRED while they run, and the ASYNCHRONOUS
 * one comes back only if they return. Limitation: only these points are
 * wrapped (pthread_testcancel(), nanosleep(), sleep(), read(), write(),
 * pause(), pthread_join(), pthread_cond_wait(), pthread_cond_timedwait());
 * a thread canceled in another one would still report ASYNCHRONOUS in its
 * cleanup handlers. The tests that declare this variant use only these.
 */
#define _GNU_SOURCE 1
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include "pcvariant.h"

static _Thread_local int async_set;	/* this thread's type is ASYNCHRONOUS, as far as it knows */

int pthread_setcanceltype(int type, int *old)
{
	const char *v = variant_of("pthread_setcancelstate.");
	int was, dummy, r;
	REAL(pthread_setcanceltype);

	if (!IS("async_deferred"))
		return real_pthread_setcanceltype(type, old);
	was = async_set ? PTHREAD_CANCEL_ASYNCHRONOUS : PTHREAD_CANCEL_DEFERRED;
	r = real_pthread_setcanceltype(type == PTHREAD_CANCEL_ASYNCHRONOUS ? PTHREAD_CANCEL_DEFERRED : type, &dummy);
	if (r == 0) {
		async_set = type == PTHREAD_CANCEL_ASYNCHRONOUS;
		if (old != NULL)
			*old = was;
	}
	return r;
}

/* Runs call with the reported type DEFERRED, as acting on a request would leave it. */
#define AROUND(call) do { saved = async_set; async_set = 0; call; async_set = saved; } while (0)

void pthread_testcancel(void)
{
	REAL(pthread_testcancel);
	int saved;

	AROUND(real_pthread_testcancel());
}

int nanosleep(const struct timespec *rq, struct timespec *rm)
{
	REAL(nanosleep);
	int saved, r;

	AROUND(r = real_nanosleep(rq, rm));
	return r;
}

unsigned sleep(unsigned s)
{
	REAL(sleep);
	int saved;
	unsigned r;

	AROUND(r = real_sleep(s));
	return r;
}

ssize_t read(int fd, void *b, size_t n)
{
	REAL(read);
	int saved;
	ssize_t r;

	AROUND(r = real_read(fd, b, n));
	return r;
}

ssize_t write(int fd, const void *b, size_t n)
{
	REAL(write);
	int saved;
	ssize_t r;

	AROUND(r = real_write(fd, b, n));
	return r;
}

int pause(void)
{
	REAL(pause);
	int saved, r;

	AROUND(r = real_pause());
	return r;
}

int pthread_join(pthread_t t, void **ret)
{
	REAL(pthread_join);
	int saved, r;

	AROUND(r = real_pthread_join(t, ret));
	return r;
}

int pthread_cond_wait(pthread_cond_t *restrict c, pthread_mutex_t *restrict m)
{
	REAL(pthread_cond_wait);
	int saved, r;

	AROUND(r = real_pthread_cond_wait(c, m));
	return r;
}

int pthread_cond_timedwait(pthread_cond_t *restrict c, pthread_mutex_t *restrict m, const struct timespec *restrict t)
{
	REAL(pthread_cond_timedwait);
	int saved, r;

	AROUND(r = real_pthread_cond_timedwait(c, m, t));
	return r;
}
