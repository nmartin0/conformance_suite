/*
 * A signal interrupts a waiting F_LOCK.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.lockf.018
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <poll.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

/* After pc_test(): a step that is not under test gives UNRESOLVED, not FAIL. */
#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

/* A regular file "f" of n bytes; returns a descriptor open O_RDWR (set-up: problems are UNRESOLVED). */
static int mkf(size_t n)
{
	char buf[256];
	int fd;

	memset(buf, 'a', sizeof buf);
	fd = open("f", O_RDWR | O_CREAT | O_TRUNC, 0600);
	if (fd < 0)
		pc_unresolved("cannot create f: %s", pc_errname(errno));
	while (n > 0) {
		size_t k = n < sizeof buf ? n : sizeof buf;

		if (write(fd, buf, k) != (ssize_t)k)
			pc_unresolved("cannot fill f: %s", pc_errname(errno));
		n -= k;
	}
	return fd;
}

/* lockf() by the calling process at an absolute offset. */
static int lk(int fd, int fn, off_t off, off_t size)
{
	if (lseek(fd, off, SEEK_SET) != off)
		pc_unresolved("lseek failed: %s", pc_errname(errno));
	return lockf(fd, fn, size);
}

/*
 * What another process sees: a child opens path itself, seeks to off and calls
 * lockf(fn, size) once, never waiting. Returns the call's result, *err its errno.
 */
static int probe_path(const char *path, int fn, off_t off, off_t size, int *err)
{
	int p[2], res[2], n;
	pid_t c;

	if (pipe(p) != 0)
		pc_unresolved("pipe failed: %s", pc_errname(errno));
	fflush(NULL);
	c = fork();
	if (c == -1)
		pc_unresolved("fork failed: %s", pc_errname(errno));
	if (c == 0) {
		int v[2] = { -2, 0 };
		int fd;

		close(p[0]);
		signal(SIGALRM, SIG_DFL);
		alarm(10);
		fd = open(path, O_RDWR);
		if (fd >= 0 && lseek(fd, off, SEEK_SET) == off) {
			v[0] = lockf(fd, fn, size);
			v[1] = errno;
		}
		if (write(p[1], v, sizeof v) != (ssize_t)sizeof v)
			_exit(2);
		_exit(0);
	}
	close(p[1]);
	n = (int)read(p[0], res, sizeof res);
	close(p[0]);
	if (waitpid(c, NULL, 0) != c)
		pc_unresolved("waitpid failed");
	if (n != (int)sizeof res || res[0] == -2)
		pc_unresolved("the probing child could not do its call");
	if (err != NULL)
		*err = res[1];
	return res[0];
}

static int probe(int fn, off_t off, off_t size, int *err)
{
	return probe_path("f", fn, off, size, err);
}

/*
 * 1 if another process finds [off, off+size) locked (F_TEST fails with
 * EACCES or EAGAIN), 0 if F_TEST returns 0; anything else is UNRESOLVED.
 */
static int locked(off_t off, off_t size)
{
	int e, r = probe(F_TEST, off, size, &e);

	if (r == 0)
		return 0;
	if (r == -1 && (e == EACCES || e == EAGAIN))
		return 1;
	pc_unresolved("F_TEST by another process gave %d, errno %s", r, pc_errname(e));
	return -1;
}

/* A call that blocks must fail the test, not hang it. */
static void on_guard(int sig)
{
	(void)sig;
	pc_fail("blocked: the call under test did not finish in time");
}

static void guard(unsigned secs)
{
	struct sigaction sa;

	memset(&sa, 0, sizeof sa);
	sa.sa_handler = on_guard;
	sigemptyset(&sa.sa_mask);
	if (sigaction(SIGALRM, &sa, NULL) != 0)
		pc_unresolved("sigaction failed");
	alarm(secs);
}


static void on_alarm(int sig)
{
	(void)sig;
}

int main(void)
{
	int fd, ann[2], res[2], v[2], st;
	struct pollfd pf;
	pid_t c;
	char b;

	pc_begin("XSH.lockf.018");
	fd = mkf(200);
	if (pipe(ann) != 0 || pipe(res) != 0)
		pc_unresolved("pipe failed: %s", pc_errname(errno));
	guard(60);
	pc_test();
	NEED(lk(fd, F_TLOCK, 0, 50) == 0, "lockf(F_TLOCK) failed: %s", pc_errname(errno));
	fflush(NULL);
	c = fork();
	NEED(c != -1, "fork failed: %s", pc_errname(errno));
	if (c == 0) {
		struct sigaction sa;
		int h = open("f", O_RDWR);

		close(ann[0]);
		close(res[0]);
		memset(&sa, 0, sizeof sa);
		sa.sa_handler = on_alarm;
		sigemptyset(&sa.sa_mask);
		v[0] = -2;
		v[1] = 0;
		if (h >= 0 && lseek(h, 0, SEEK_SET) == 0 && sigaction(SIGALRM, &sa, NULL) == 0) {
			if (write(ann[1], "a", 1) != 1)
				_exit(2);
			alarm(1);
			v[0] = lockf(h, F_LOCK, 50);
			v[1] = errno;
		}
		if (write(res[1], v, sizeof v) != (ssize_t)sizeof v)
			_exit(3);
		_exit(0);
	}
	close(ann[1]);
	close(res[1]);
	pf.fd = ann[0];
	pf.events = POLLIN;
	NEED(poll(&pf, 1, 10000) == 1 && read(ann[0], &b, 1) == 1, "the child did not start in 10 s");
	pf.fd = res[0];
	pf.events = POLLIN;
	if (poll(&pf, 1, 12000) != 1) {
		kill(c, SIGKILL);
		waitpid(c, &st, 0);
		pc_fail("the waiting F_LOCK was not interrupted by a caught SIGALRM within 12 s");
	}
	NEED(read(res[0], v, sizeof v) == (ssize_t)sizeof v, "no result from the child");
	NEED(waitpid(c, &st, 0) == c, "waitpid failed");
	NEED(v[0] != -2, "the child could not set up");
	pc_check(v[0] == -1 && v[1] == EINTR, "F_LOCK returned %d, errno %s, expected -1 with EINTR", v[0], pc_errname(v[1]));
	pc_pass();
}
