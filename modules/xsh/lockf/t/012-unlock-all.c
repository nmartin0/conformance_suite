/*
 * F_ULOCK releases the section.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.lockf.012
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


int main(void)
{
	int fd, e, r;

	pc_begin("XSH.lockf.012");
	fd = mkf(200);
	pc_test();
	NEED(lk(fd, F_TLOCK, 10, 30) == 0, "lockf(F_TLOCK) failed: %s", pc_errname(errno));
	NEED(locked(10, 30) == 1, "the lock set up is not seen by another process");
	pc_check(lk(fd, F_ULOCK, 10, 30) == 0, "lockf(F_ULOCK) failed: %s", pc_errname(errno));
	r = probe(F_TEST, 10, 30, &e);
	pc_check(r == 0, "F_TEST after F_ULOCK returned %d, errno %s", r, pc_errname(e));
	r = probe(F_TLOCK, 10, 30, &e);
	pc_check(r == 0, "F_TLOCK by another process after F_ULOCK returned %d, errno %s", r, pc_errname(e));
	pc_pass();
}
