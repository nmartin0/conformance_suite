/*
 * LIO_WAIT with a write, a read of a file and a read of a pipe (fed by
 * another thread): lio_listio() returns 0 only when all have completed, and
 * the sig argument is ignored.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.lio_listio.001
 * MUTANT    lio_listio.wait_as_nowait
 * MUTANT    lio_listio.sig_not_ignored
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
static struct aiocb cb[3];
static struct aiocb *list[3];
static char rbuf[8], pbuf[2];
static int wfd;
static volatile sig_atomic_t usr1;

static void hdl(int s)
{
	(void)s;
	usr1++;
}

static void *writer(void *a)
{
	(void)a;
	sleep_ms(200);
	(void)write(wfd, "q", 1);
	return NULL;
}

int main(void)
{
	struct sigaction sa;
	struct sigevent sev;
	pthread_t t;
	int p[2], r, i, e, rd, wr;

	pc_begin("XSH.lio_listio.001");
	guard();
	pc_make_file("r", "abcdefgh");
	pc_make_file("w", "");
	rd = open("r", O_RDONLY);
	wr = open("w", O_WRONLY);
	pc_check(rd >= 0 && wr >= 0, "cannot open the files");
	pc_check(pipe(p) == 0, "pipe failed");
	wfd = p[1];
	release_at_exit(p[1]);
	memset(&sa, 0, sizeof sa);
	sa.sa_handler = hdl;
	sigemptyset(&sa.sa_mask);
	pc_check(sigaction(SIGUSR1, &sa, NULL) == 0, "sigaction failed");
	mkcb(&cb[0], wr, "hello", 5, 0);
	cb[0].aio_lio_opcode = LIO_WRITE;
	mkcb(&cb[1], rd, rbuf, 4, 2);
	cb[1].aio_lio_opcode = LIO_READ;
	mkcb(&cb[2], p[0], pbuf, 1, 0);
	cb[2].aio_lio_opcode = LIO_READ;
	for (i = 0; i < 3; i++)
		list[i] = &cb[i];
	memset(&sev, 0, sizeof sev);
	sev.sigev_notify = SIGEV_SIGNAL;
	sev.sigev_signo = SIGUSR1;
	go();
	if (pthread_create(&t, NULL, writer, NULL) != 0)
		pc_unresolved("pthread_create failed");
	must_return = 1;	/* LIO_WAIT returns once all the I/O is complete */
	r = lio_listio(LIO_WAIT, list, 3, &sev);
	must_return = 0;
	if (r == -1 && errno == ENOSYS)
		pc_notinuse("asynchronous I/O is not implemented (ENOSYS)");
	if (r == -1 && errno == EAGAIN)
		pc_unresolved("lio_listio: no resources (EAGAIN)");
	pc_check(r == 0, "lio_listio returned %d (%s), expected 0", r, r == -1 ? pc_errname(errno) : "-");
	for (i = 0; i < 3; i++) {
		e = aio_error(&cb[i]);
		pc_check(e == 0, "request %d: aio_error is %d (%s) as lio_listio returned", i, e, pc_errname(e));
	}
	pc_check(aio_return(&cb[0]) == 5, "the write's aio_return is not 5");
	pc_check(aio_return(&cb[1]) == 4, "the file read's aio_return is not 4");
	pc_check(aio_return(&cb[2]) == 1, "the pipe read's aio_return is not 1");
	pc_check(memcmp(rbuf, "cdef", 4) == 0 && pbuf[0] == 'q', "wrong data read");
	pc_check(pc_file_equals("w", "hello"), "the file does not hold hello");
	(void)pthread_join(t, NULL);
	sleep_ms(100);
	pc_check(usr1 == 0, "a notification was sent although LIO_WAIT ignores sig");
	pc_pass();
}
