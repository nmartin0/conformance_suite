/*
 * No function sets errno to zero.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.errno.004
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define CHECK(call, name) do { errno = ERANGE; (void)(call); \
	pc_check(errno != 0, name "() set errno to zero"); } while (0)

int main(void)
{
	char buf[256];
	int fd, p[2], d;
	void *m;
	sigset_t set;

	pc_begin("XSH.errno.004");
	pc_make_file("e-file", "0123456789");
	pc_test();
	CHECK(getpid(), "getpid");
	CHECK(strlen("abc"), "strlen");
	errno = ERANGE;
	m = malloc(100);
	if (m == NULL)
		pc_unresolved("malloc failed");
	pc_check(errno != 0, "malloc() set errno to zero");
	errno = ERANGE;
	free(m);
	pc_check(errno != 0, "free() set errno to zero");
	errno = ERANGE;
	fd = open("e-file", O_RDWR);
	if (fd < 0)
		pc_unresolved("open failed");
	pc_check(errno != 0, "open() set errno to zero");
	CHECK(read(fd, buf, 4), "read");
	CHECK(lseek(fd, 0, SEEK_SET), "lseek");
	CHECK(write(fd, "ab", 2), "write");
	CHECK(fcntl(fd, F_GETFD), "fcntl");
	errno = ERANGE;
	d = dup(fd);
	if (d < 0)
		pc_unresolved("dup failed");
	pc_check(errno != 0, "dup() set errno to zero");
	(void)close(d);
	CHECK(close(fd), "close");
	errno = ERANGE;
	if (pipe(p) != 0)
		pc_unresolved("pipe failed");
	pc_check(errno != 0, "pipe() set errno to zero");
	(void)close(p[0]);
	(void)close(p[1]);
	CHECK(getcwd(buf, sizeof buf), "getcwd");
	CHECK(sysconf(_SC_PAGESIZE), "sysconf");
	CHECK(time(NULL), "time");
	sigemptyset(&set);
	CHECK(sigprocmask(SIG_BLOCK, &set, NULL), "sigprocmask");
	pc_pass();
}
