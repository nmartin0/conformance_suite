/*
 * Async-signal-safe functions work when called from a handler that interrupts them.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.4.3-signal-actions.016
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <setjmp.h>
#include <signal.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

static void catch_sig(int sig, void (*fn)(int))
{
	struct sigaction sa;

	memset(&sa, 0, sizeof sa);
	sa.sa_handler = fn;
	sigemptyset(&sa.sa_mask);
	pc_check(sigaction(sig, &sa, NULL) == 0, "sigaction(%d) failed", sig);
}

static void catch_info(int sig, void (*fn)(int, siginfo_t *, void *))
{
	struct sigaction sa;

	memset(&sa, 0, sizeof sa);
	sa.sa_sigaction = fn;
	sa.sa_flags = SA_SIGINFO;
	sigemptyset(&sa.sa_mask);
	pc_check(sigaction(sig, &sa, NULL) == 0, "sigaction(%d) failed", sig);
}

/* Sets SIG_DFL or SIG_IGN (or a plain handler) with sigaction(). */
static void set_action(int sig, void (*fn)(int))
{
	catch_sig(sig, fn);
}

static void mask(int how, int sig)
{
	sigset_t s;

	sigemptyset(&s);
	sigaddset(&s, sig);
	pc_check(pthread_sigmask(how, &s, NULL) == 0, "pthread_sigmask failed");
}

static int is_pending(int sig)
{
	sigset_t p;

	pc_check(sigpending(&p) == 0, "sigpending failed");
	return sigismember(&p, sig) == 1;
}

static void sleep_ms(long ms)
{
	struct timespec ts;

	ts.tv_sec = ms / 1000;
	ts.tv_nsec = (ms % 1000) * 1000000L;
	while (nanosleep(&ts, &ts) == -1 && errno == EINTR)
		;
}

/* Children made here are killed when the test exits, however it ends. */
static pid_t kids[16];
static int nkids;

static void kill_kids(void)
{
	int i;

	for (i = 0; i < nkids; i++)
		if (kids[i] > 0) {
			(void)kill(kids[i], SIGKILL);
			(void)kill(kids[i], SIGCONT);
		}
}

static void track(pid_t c)
{
	if (nkids == 0)
		(void)atexit(kill_kids);
	if (nkids < 16)
		kids[nkids++] = c;
}

static void kill_child(pid_t c)
{
	(void)kill(c, SIGKILL);
	(void)kill(c, SIGCONT);
	while (waitpid(c, NULL, 0) == -1 && errno == EINTR)
		;
}

/*
 * Waits up to ms for a status change of c that opts select (WUNTRACED
 * adds stops); 1 with *st set, or 0 if none came in time.
 */
static int wait_by(pid_t c, int opts, long ms, int *st)
{
	long waited;
	pid_t r;

	for (waited = 0; waited <= ms; waited += 10) {
		r = waitpid(c, st, opts | WNOHANG);
		if (r == c)
			return 1;
		if (r == -1 && errno != EINTR)
			pc_check(0, "waitpid failed: %s", pc_errname(errno));
		sleep_ms(10);
	}
	return 0;
}

/* The status of c once it ends (or stops); FAIL if neither within 10 s. */
static int end_of(pid_t c, const char *what)
{
	int st = 0;

	if (!wait_by(c, WUNTRACED, 10000, &st)) {
		kill_child(c);
		pc_check(0, "%s: the child did not end within 10 s", what);
	}
	if (WIFSTOPPED(st))
		kill_child(c);
	return st;
}

/* Waits until c is stopped by sig; FAIL (child killed) otherwise. */
static void await_stop(pid_t c, int sig)
{
	int st = 0;

	if (!wait_by(c, WUNTRACED, 10000, &st)) {
		kill_child(c);
		pc_check(0, "the child was not stopped within 10 s");
	}
	if (!WIFSTOPPED(st)) {
		pc_check(0, "the child ended (status %#x) instead of stopping", st);
	}
	if (WSTOPSIG(st) != sig) {
		kill_child(c);
		pc_check(0, "the child was stopped by %d, expected %d", WSTOPSIG(st), sig);
	}
}

/* Reads one byte from fd within ms milliseconds: the byte, or -1. */
static int read_within(int fd, long ms)
{
	long waited;
	char ch;
	int fl = fcntl(fd, F_GETFL);

	(void)fcntl(fd, F_SETFL, fl | O_NONBLOCK);
	for (waited = 0; waited <= ms; waited += 10) {
		ssize_t k = read(fd, &ch, 1);
		if (k == 1) {
			(void)fcntl(fd, F_SETFL, fl);
			return (unsigned char)ch;
		}
		if (k == 0)
			break;
		sleep_ms(10);
	}
	(void)fcntl(fd, F_SETFL, fl);
	return -1;
}

/* Waits (no deadline: the driver's limit covers a hang) for c's 'a'. */
static void started(int fd, pid_t c)
{
	char ch = 0;
	ssize_t k;

	while ((k = read(fd, &ch, 1)) == -1 && errno == EINTR)
		;
	if (k != 1 || ch != 'a') {
		kill_child(c);
		pc_unresolved("the child did not start (%c)", k == 1 ? ch : '-');
	}
}

/*
 * Runs fn in a child (in the test's process group, which is not orphaned)
 * and returns its pid; report[0] is the read end of a pipe whose write end
 * the child has as fd report[1].
 */
static pid_t child(void (*fn)(int), int report[2])
{
	pid_t c;

	pc_check(pipe(report) == 0, "pipe failed");
	c = fork();
	pc_check(c != -1, "fork failed");
	if (c == 0) {
		(void)close(report[0]);
		fn(report[1]);
		_exit(0);
	}
	track(c);
	(void)close(report[1]);
	return c;
}

static void say(int fd, char ch)
{
	(void)write(fd, &ch, 1);
}

static pid_t pid;
static int devnull;
static char path[] = "pcasync.txt";
static const char text[] = "posixcheck async-signal-safe";
static volatile sig_atomic_t calls, bad;

/* Runs the sample once; returns 0 if all is well, else a step number. */
static int sample(void)
{
	sigset_t m;
	struct sigaction a;
	struct timespec t1, t2;
	struct stat st;
	char buf[64], copy[64];
	int fd;
	ssize_t k;

	if (getpid() != pid)
		return 1;
	if (kill(pid, 0) != 0)
		return 2;
	if (sigprocmask(SIG_BLOCK, NULL, &m) != 0)
		return 3;
	if (sigaction(SIGUSR1, NULL, &a) != 0 || a.sa_handler != SIG_DFL)
		return 4;
	if (clock_gettime(CLOCK_REALTIME, &t1) != 0 || clock_gettime(CLOCK_REALTIME, &t2) != 0)
		return 5;
	if (t2.tv_sec < t1.tv_sec || (t2.tv_sec == t1.tv_sec && t2.tv_nsec < t1.tv_nsec))
		return 6;
	if (time(NULL) == (time_t)-1)
		return 7;
	fd = open(path, O_RDONLY);
	if (fd == -1)
		return 8;
	k = read(fd, buf, sizeof buf);
	if (fstat(fd, &st) != 0 || st.st_size != (off_t)(sizeof text - 1)) {
		(void)close(fd);
		return 9;
	}
	if (close(fd) != 0)
		return 10;
	if (k != (ssize_t)(sizeof text - 1) || memcmp(buf, text, sizeof text - 1) != 0)
		return 11;
	memcpy(copy, buf, (size_t)k);
	copy[k] = '\0';
	if (strlen(copy) != sizeof text - 1)
		return 12;
	if (write(devnull, copy, (size_t)k) != k)
		return 13;
	return 0;
}

static void h(int sig)
{
	int e = errno, r;

	(void)sig;
	r = sample();
	if (r != 0 && bad == 0)
		bad = r;
	calls = calls + 1;
	errno = e;
}

int main(void)
{
	struct sigevent ev;
	struct itimerspec it;
	timer_t tm;
	struct timespec start, now;
	long n = 0;
	int r = 0;

	pc_begin("XSH.2.4.3-signal-actions.016");
	pid = getpid();
	pc_require_space();
	pc_make_file(path, text);
	devnull = open("/dev/null", O_WRONLY);
	pc_check(devnull != -1, "open /dev/null failed");
	catch_sig(SIGALRM, h);
	memset(&ev, 0, sizeof ev);
	ev.sigev_notify = SIGEV_SIGNAL;
	ev.sigev_signo = SIGALRM;
	pc_check(timer_create(CLOCK_REALTIME, &ev, &tm) == 0, "timer_create failed");
	memset(&it, 0, sizeof it);
	it.it_value.tv_nsec = 1000000;
	it.it_interval.tv_nsec = 1000000;
	pc_test();
	pc_check(timer_settime(tm, 0, &it, NULL) == 0, "timer_settime failed");
	(void)clock_gettime(CLOCK_MONOTONIC, &start);
	do {
		r = sample();
		n++;
		(void)clock_gettime(CLOCK_MONOTONIC, &now);
	} while (r == 0 && bad == 0 && (now.tv_sec - start.tv_sec) * 1000 + (now.tv_nsec - start.tv_nsec) / 1000000 < 1000);
	(void)timer_delete(tm);
	pc_check(r == 0, "in the main program, step %d of the sample failed (after %ld rounds)", r, n);
	pc_check(bad == 0, "in the handler, step %d of the sample failed", (int)bad);
	pc_check(calls >= 100, "the handler ran only %d times in a second", (int)calls);
	pc_pass();
}
