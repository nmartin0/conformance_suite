/*
 * fgetpos() on a pipe, a FIFO or a socket fails with ESPIPE.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fgetpos.003
 * MUTANT    fgetpos.espipe_zero
 * MUTANT    fgetpos.wrong_errno
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

static void fails(int fd, const char *what)
{
	fpos_t p;
	FILE *f = fdopen(fd, "r");
	int r, e;

	NEED(f != NULL, "fdopen() of the %s failed: %s", what, pc_errname(errno));
	errno = 0;
	r = fgetpos(f, &p);
	e = errno;
	pc_check(r != 0, "fgetpos() on the %s returned 0", what);
	pc_check(e == ESPIPE, "fgetpos() on the %s failed with %s, expected ESPIPE", what, pc_errname(e));
}

int main(void)
{
	pc_begin("XSH.fgetpos.003");
	{
		int p[2], s[2], q;

		pc_check(pipe(p) == 0, "pipe failed");
		pc_check(mkfifo("fifo", 0600) == 0, "mkfifo failed");
		q = open("fifo", O_RDONLY | O_NONBLOCK);
		pc_check(q >= 0, "open of the FIFO failed");
		if (!(socketpair(AF_UNIX, SOCK_STREAM, 0, s) == 0))
			pc_unresolved("socketpair failed");
		pc_test();
		fails(p[0], "pipe");
		fails(q, "FIFO");
		fails(s[0], "socket");
	}
	pc_pass();
}
