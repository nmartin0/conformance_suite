/*
 * fsetpos() whose flush passes the file size limit gives EFBIG.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fsetpos.009
 * MUTANT    fsetpos.wrong_errno
 * MUTANT    fsetpos.error_cleared
 * VARIANT   fsetpos.lseek_first
 * VARIANT   setvbuf.own_unused
 * VARIANT   setvbuf.scribbles
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <signal.h>
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
#include <wchar.h>

/* After pc_test(): a step that is not under test gives UNRESOLVED, not FAIL. */
#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

static FILE *open_as(const char *path, const char *mode)
{
	FILE *f = fopen(path, mode);

	NEED(f != NULL, "fopen(%s, %s) failed: %s", path, mode, pc_errname(errno));
	return f;
}

/* The file's whole contents (at most size - 1 bytes) and its length. */
static long slurp(const char *path, char *buf, size_t size)
{
	int fd = open(path, O_RDONLY);
	long n = 0, r;

	NEED(fd >= 0, "open(%s) failed: %s", path, pc_errname(errno));
	while ((size_t)n < size - 1 && (r = read(fd, buf + n, size - 1 - (size_t)n)) > 0)
		n += r;
	(void)close(fd);
	buf[n] = '\0';
	return n;
}

static int ts_after(struct timespec a, struct timespec b)
{
	return a.tv_sec > b.tv_sec || (a.tv_sec == b.tv_sec && a.tv_nsec > b.tv_nsec);
}

/*
 * Set the error indicator of a stream that has nothing buffered for input:
 * its descriptor is closed for one fgetc(), whose read fails with EBADF
 * (XSH fgetc(): data needs to be read), then put back with dup2(). The
 * copy is never used to affect the offset, so it is not a second handle
 * (XSH 2.5.1).
 */
static void set_error(FILE *f)
{
	int fd = fileno(f), d = dup(fd);

	NEED(fd >= 0 && d >= 0, "dup failed: %s", pc_errname(errno));
	NEED(close(fd) == 0, "close failed");
	NEED(fgetc(f) == EOF, "fgetc() on a closed descriptor returned a byte");
	NEED(dup2(d, fd) == fd && close(d) == 0, "dup2 failed: %s", pc_errname(errno));
	NEED(ferror(f), "fgetc() on a closed descriptor did not set the error indicator");
}

static int child(void)
{
	static char vb[4096];
	struct rlimit rl;
	sigset_t s;
	fpos_t p0;
	FILE *f;
	int i, r, e;

	sigemptyset(&s);
#ifdef SIGXFSZ	/* XSI: without it there is no signal to block */
	sigaddset(&s, SIGXFSZ);
#endif
	if (sigprocmask(SIG_BLOCK, &s, NULL) != 0 || getrlimit(RLIMIT_FSIZE, &rl) != 0)
		return 99;
	if (rl.rlim_max != RLIM_INFINITY && rl.rlim_max < 100)
		return 98;
	rl.rlim_cur = 100;
	if (setrlimit(RLIMIT_FSIZE, &rl) != 0)
		return 98;
	f = fopen("big", "w");
	if (f == NULL || setvbuf(f, vb, _IOFBF, sizeof vb) != 0 || fgetpos(f, &p0) != 0)
		return 97;
	for (i = 0; i < 200; i++)
		if (fputc('a', f) == EOF)
			return 96;
	errno = 0;
	r = fsetpos(f, &p0);
	e = errno;
	if (r == 0)
		return 1;
	if (e != EFBIG)
		return 2;
	if (!ferror(f))
		return 3;
#if defined(_XOPEN_UNIX) && _XOPEN_UNIX != -1 && defined(SIGXFSZ)
	if (sigpending(&s) != 0 || !sigismember(&s, SIGXFSZ))
		return 4;
#endif
	return 0;
}

int main(void)
{
	pc_begin("XSH.fsetpos.009");
	{
		pid_t kid;
		int st;

		pc_test();
		kid = fork();
		NEED(kid != -1, "fork failed");
		if (kid == 0)
			_exit(child());
		NEED(waitpid(kid, &st, 0) == kid, "waitpid failed");
		NEED(WIFEXITED(st) && WEXITSTATUS(st) < 90, "the child could not run (status %#x)", (unsigned)st);
		pc_check(WEXITSTATUS(st) != 1, "fsetpos() succeeded beyond the file size limit");
		pc_check(WEXITSTATUS(st) != 2, "fsetpos() failed, but not with EFBIG");
		pc_check(WEXITSTATUS(st) != 3, "the error indicator is not set");
		pc_check(WEXITSTATUS(st) != 4, "SIGXFSZ is not pending");
		pc_check(WEXITSTATUS(st) == 0, "child status %d", WEXITSTATUS(st));
	}
	pc_pass();
}
