/*
 * aio_fsync() with O_DSYNC completes with error status 0 and return status 0,
 * the data written before it is intact, and the control block's other
 * members are ignored.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.aio_fsync.001
 * MUTANT    aio_fsync.rejects_op
 * MUTANT    aio_fsync.junk_member_einval
 */
#include "pc.h"
#include <aio.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

static volatile sig_atomic_t in_test, must_return;

static void on_alarm(int s)
{
	(void)s;
	/* FAIL only where the text makes not returning itself a violation. */
	if (in_test && must_return)
		pc_fail("a call that must return did not return within the 10 s guard time");
	pc_unresolved("a wait or the setup did not end within the 10 s guard time (no bound is stated)");
}

/* Bounds every wait of the test: a stuck implementation ends the run. */
static inline void guard(void)
{
	struct sigaction sa;

	memset(&sa, 0, sizeof sa);
	sa.sa_handler = on_alarm;
	sigemptyset(&sa.sa_mask);
	(void)sigaction(SIGALRM, &sa, NULL);
	(void)alarm(10);
}

static inline void go(void)
{
	in_test = 1;
	pc_test();
}

static inline void sleep_ms(long ms)
{
	struct timespec ts;

	ts.tv_sec = ms / 1000;
	ts.tv_nsec = (ms % 1000) * 1000000L;
	while (nanosleep(&ts, &ts) == -1 && errno == EINTR)
		;
}

static inline long long now_ns(void)
{
	struct timespec t;

	(void)clock_gettime(CLOCK_MONOTONIC, &t);
	return (long long)t.tv_sec * 1000000000LL + t.tv_nsec;
}

/* A submission that must succeed: no support is NOTINUSE, no resources UNRESOLVED. */
static inline void queued(int r, const char *what)
{
	if (r == 0)
		return;
	if (errno == ENOSYS)
		pc_notinuse("%s: asynchronous I/O is not implemented (ENOSYS)", what);
	if (errno == EAGAIN)
		pc_unresolved("%s: no resources (EAGAIN)", what);
	pc_check(0, "%s failed: %s", what, pc_errname(errno));
}

/* Control block for fd, buffer, length and offset; no notification. */
static inline void mkcb(struct aiocb *cb, int fd, void *buf, size_t n, off_t off)
{
	memset(cb, 0, sizeof *cb);
	cb->aio_fildes = fd;
	cb->aio_buf = buf;
	cb->aio_nbytes = n;
	cb->aio_offset = off;
	cb->aio_sigevent.sigev_notify = SIGEV_NONE;
}

/* Polls until the request is no longer in progress; 0 on timeout. */
static inline int wait_status(struct aiocb *cb, long ms)
{
	long long end = now_ns() + ms * 1000000LL;

	while (aio_error(cb) == EINPROGRESS) {
		if (now_ns() > end)
			return 0;
		sleep_ms(2);
	}
	return 1;
}

/* As wait_status(), but the text bounds no completion time: a timeout is UNRESOLVED. */
static inline int wait_done(struct aiocb *cb, long ms)
{
	if (!wait_status(cb, ms))
		pc_unresolved("a request was still in progress after %ld ms (no bound is stated)", ms);
	return 1;
}

static int rel_fd = -1;

static void rel_write(void)
{
	if (rel_fd >= 0) {
		ssize_t n = write(rel_fd, "xx", 2);

		(void)n;
	}
}

/* Frees a read of an empty pipe still pending when the test ends. */
static inline void release_at_exit(int wfd)
{
	rel_fd = wfd;
	(void)atexit(rel_write);
}

/*
 * Holds a one-byte read of an empty pipe in progress (setup phase only):
 * it completes when a byte is written to p[1].
 */
static inline void hold(struct aiocb *cb, int p[2], char *buf)
{
	pc_check(pipe(p) == 0, "pipe failed");
	release_at_exit(p[1]);
	mkcb(cb, p[0], buf, 1, 0);
	queued(aio_read(cb), "aio_read of an empty pipe");
	pc_check(aio_error(cb) == EINPROGRESS, "a read of an empty pipe is not in progress");
}
static struct aiocb wcb, scb;

int main(void)
{
	int fd, e, r;

	pc_begin("XSH.aio_fsync.001");
	guard();
	pc_make_file("f", "");
	fd = open("f", O_WRONLY);
	pc_check(fd >= 0, "cannot open f");
	mkcb(&wcb, fd, "hello", 5, 0);
	memset(&scb, 0, sizeof scb);
	scb.aio_fildes = fd;
	scb.aio_sigevent.sigev_notify = SIGEV_NONE;
	scb.aio_nbytes = 1000000;
	scb.aio_buf = NULL;
	scb.aio_offset = -5;
	scb.aio_lio_opcode = LIO_READ;
	go();
	queued(aio_write(&wcb), "aio_write");
	pc_check(wait_done(&wcb, 8000), "the write did not complete");
	pc_check(aio_return(&wcb) == 5, "the write's aio_return is not 5");
	r = aio_fsync(O_DSYNC, &scb);
	if (r == -1 && errno == EINVAL)
		pc_unresolved("aio_fsync: synchronized I/O is not supported for this file (EINVAL)");
	queued(r, "aio_fsync");
	pc_check(wait_done(&scb, 8000), "the synchronization did not complete");
	e = aio_error(&scb);
	pc_check(e == 0, "aio_error is %d (%s), expected 0", e, pc_errname(e));
	pc_check(aio_return(&scb) == 0, "aio_return is not 0");
	pc_check(pc_file_equals("f", "hello"), "the file does not hold the data written");
	pc_pass();
}
