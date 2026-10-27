/*
 * LIO_NOWAIT with sig: the notification comes only when all the requests
 * have completed, and carries the sigev_value pointer.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.lio_listio.003
 * MUTANT    lio_listio.notify_early
 * MUTANT    lio_listio.value_lost
 * MUTANT    lio_listio.no_notify
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
static struct aiocb cb[2];
static struct aiocb *list[2];
static char fbuf[8], pbuf[2];
static int token;
static volatile sig_atomic_t got, gsig;
static void *volatile gval;

static void hdl(int sig, siginfo_t *si, void *uc)
{
	(void)uc;
	gsig = sig;
	gval = si->si_value.sival_ptr;
	got = 1;
}

int main(void)
{
	struct sigaction sa;
	struct sigevent sev;
	int p[2], fd, i, r, e;

	pc_begin("XSH.lio_listio.003");
	guard();
	pc_make_file("f", "0123456789");
	fd = open("f", O_RDONLY);
	pc_check(fd >= 0, "cannot open f");
	pc_check(pipe(p) == 0, "pipe failed");
	release_at_exit(p[1]);
	memset(&sa, 0, sizeof sa);
	sa.sa_sigaction = hdl;
	sa.sa_flags = SA_SIGINFO;
	sigemptyset(&sa.sa_mask);
	pc_check(sigaction(SIGRTMIN, &sa, NULL) == 0, "sigaction failed");
	mkcb(&cb[0], fd, fbuf, 4, 0);
	cb[0].aio_lio_opcode = LIO_READ;
	mkcb(&cb[1], p[0], pbuf, 1, 0);
	cb[1].aio_lio_opcode = LIO_READ;
	list[0] = &cb[0];
	list[1] = &cb[1];
	memset(&sev, 0, sizeof sev);
	sev.sigev_notify = SIGEV_SIGNAL;
	sev.sigev_signo = SIGRTMIN;
	sev.sigev_value.sival_ptr = &token;
	go();
	must_return = 1;	/* LIO_NOWAIT returns at once */
	r = lio_listio(LIO_NOWAIT, list, 2, &sev);
	must_return = 0;
	if (r == -1 && errno == ENOSYS)
		pc_notinuse("asynchronous I/O is not implemented (ENOSYS)");
	if (r == -1 && errno == EAGAIN)
		pc_unresolved("lio_listio: no resources (EAGAIN)");
	pc_check(r == 0, "lio_listio returned %d (%s), expected 0", r, r == -1 ? pc_errname(errno) : "-");
	pc_check(wait_done(&cb[0], 8000), "the file read did not complete");
	sleep_ms(300);
	pc_check(!got, "the notification came while the pipe read was still pending");
	pc_check(write(p[1], "q", 1) == 1, "write failed");
	for (i = 0; i < 800 && !got; i++)
		sleep_ms(10);
	if (!got && (aio_error(&cb[0]) == EINPROGRESS || aio_error(&cb[1]) == EINPROGRESS))
		pc_unresolved("the requests had not completed after 8 s (no bound is stated)");
	pc_check(got, "no notification after all the requests completed");
	pc_check(gsig == SIGRTMIN, "the signal is %d, not SIGRTMIN", (int)gsig);
	pc_check(gval == (void *)&token, "the signal's value is not the sigev_value pointer");
	for (i = 0; i < 2; i++) {
		e = aio_error(&cb[i]);
		pc_check(e == 0, "request %d: aio_error is %d (%s) at the notification", i, e, pc_errname(e));
	}
	pc_check(aio_return(&cb[0]) == 4 && aio_return(&cb[1]) == 1, "wrong aio_return values");
	pc_pass();
}
