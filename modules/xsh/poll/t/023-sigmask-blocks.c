/*
 * A signal blocked by ppoll()'s mask does not shorten the call.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.poll.023
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <poll.h>
#include <signal.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/un.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

/* After pc_test(): a step that is not under test gives UNRESOLVED, not FAIL. */
#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)
#define MS 1000000LL

/* A call that blocks must not hang the test. Also makes sure SIGCHLD has its default action, so that children can be waited for. */
static void on_guard(int sig)
{
	(void)sig;
	pc_unresolved("a call did not finish within the guard time");
}

static void guard(unsigned secs)
{
	struct sigaction sa;

	memset(&sa, 0, sizeof sa);
	sa.sa_handler = on_guard;
	sigemptyset(&sa.sa_mask);
	NEED(sigaction(SIGALRM, &sa, NULL) == 0, "sigaction failed");
	(void)signal(SIGCHLD, SIG_DFL);
	alarm(secs);
}

static long long mono(void)
{
	struct timespec t;

	NEED(clock_gettime(CLOCK_MONOTONIC, &t) == 0, "clock_gettime failed: %s", pc_errname(errno));
	return (long long)t.tv_sec * 1000000000LL + t.tv_nsec;
}

static long long mono_res(void)
{
	struct timespec r;

	NEED(clock_getres(CLOCK_MONOTONIC, &r) == 0, "clock_getres failed: %s", pc_errname(errno));
	return (long long)r.tv_sec * 1000000000LL + r.tv_nsec;
}

static void mkpipe(int p[2])
{
	NEED(pipe(p) == 0, "pipe failed: %s", pc_errname(errno));
}

static void putbyte(int fd)
{
	NEED(write(fd, "x", 1) == 1, "write failed: %s", pc_errname(errno));
}

static void nap_ms(long ms)
{
	struct timespec ts;

	ts.tv_sec = ms / 1000;
	ts.tv_nsec = (ms % 1000) * 1000000L;
	while (nanosleep(&ts, &ts) == -1 && errno == EINTR)
		;
}

/* v 0: poll(), v 1: ppoll() with a null mask; ms < 0: no timeout. */
static const char *const fname[2] = { "poll", "ppoll" };

static int callpoll(int v, struct pollfd *f, nfds_t n, long long ms)
{
	if (v == 0) {
		return poll(f, n, (int)ms);
	} else {
		struct timespec ts;

		if (ms < 0)
			return ppoll(f, n, NULL, NULL);
		ts.tv_sec = (time_t)(ms / 1000);
		ts.tv_nsec = (long)((ms % 1000) * 1000000L);
		return ppoll(f, n, &ts, NULL);
	}
}

#define ERRS(n, e) ((n) == -1 ? pc_errname(e) : "-")

static volatile sig_atomic_t caught;

static void on_sig(int sig)
{
	(void)sig;
	caught++;
}

/* Handler without SA_RESTART. */
static void catch_sig(int sig)
{
	struct sigaction sa;

	memset(&sa, 0, sizeof sa);
	sa.sa_handler = on_sig;
	sigemptyset(&sa.sa_mask);
	NEED(sigaction(sig, &sa, NULL) == 0, "sigaction(%d) failed", sig);
}

/* A child that sends sig to this process after first_ms and then every every_ms. */
static pid_t send_repeat(int sig, long first_ms, long every_ms)
{
	pid_t parent = getpid(), c = fork();

	NEED(c != -1, "fork failed: %s", pc_errname(errno));
	if (c == 0) {
		nap_ms(first_ms);
		for (;;) {
			if (kill(parent, sig) != 0 || getppid() != parent)
				_exit(0);
			nap_ms(every_ms);
		}
	}
	return c;
}

static void stop_sender(pid_t c)
{
	(void)kill(c, SIGKILL);
	while (waitpid(c, NULL, 0) == -1 && errno == EINTR)
		;
}

static sigset_t base_mask;

/* Remembers the signal mask the caller has now. */
static void save_mask(void)
{
	NEED(sigprocmask(SIG_BLOCK, NULL, &base_mask) == 0, "sigprocmask failed");
}

/* Is the mask what save_mask() saw, among the signals examined? */
static int mask_unchanged(void)
{
	static const int sigs[] = { SIGUSR1, SIGUSR2, SIGINT, SIGTERM, SIGALRM, SIGCHLD, SIGHUP, SIGQUIT, SIGPIPE };
	sigset_t cur;
	size_t i;

	NEED(sigprocmask(SIG_BLOCK, NULL, &cur) == 0, "sigprocmask failed");
	for (i = 0; i < sizeof sigs / sizeof sigs[0]; i++)
		if ((sigismember(&cur, sigs[i]) == 1) != (sigismember(&base_mask, sigs[i]) == 1))
			return 0;
	return 1;
}

static int is_blocked(int sig)
{
	sigset_t cur;

	NEED(sigprocmask(SIG_BLOCK, NULL, &cur) == 0, "sigprocmask failed");
	return sigismember(&cur, sig) == 1;
}

int main(void)
{
	int a[2], n, e;
	struct pollfd f;
	sigset_t blk;
	struct timespec ts;
	long long t0, got, tol;
	pid_t c;

	pc_begin("XSH.poll.023");
	guard(60);
	catch_sig(SIGUSR1);
	sigemptyset(&blk);
	sigaddset(&blk, SIGUSR1);
	tol = mono_res();
	mkpipe(a);
	f.fd = a[0]; f.events = POLLIN; f.revents = 0;
	ts.tv_sec = 0;
	ts.tv_nsec = 300 * 1000000L;
	pc_test();
	c = send_repeat(SIGUSR1, 0, 10);
	t0 = mono();
	errno = 0;
	n = ppoll(&f, 1, &ts, &blk);
	got = mono() - t0;
	e = errno;
	stop_sender(c);
	pc_check(n == 0 || (n == -1 && e == EINTR), "ppoll returned %d (errno %s)", n, pc_errname(e));
	pc_check(got + tol >= 300 * MS, "ppoll returned after %lld ms although the only signals were blocked by its mask and the timeout is 300 ms", got / MS);
	pc_pass();
}
