/*
 * ttyname() and ttyname_r() fail for -1 and a closed descriptor.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.ttyname.007
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

/* Descriptors that are not terminals: a regular file, both ends of a pipe, /dev/null. */
static int nt_fd[4];
static const char *const nt_name[4] = { "regular file", "pipe read end", "pipe write end", "/dev/null" };

static void open_nonttys(void)
{
	int p[2];

	pc_make_file("plain", "x");
	nt_fd[0] = open("plain", O_RDWR);
	if (nt_fd[0] < 0)
		pc_unresolved("open(plain) failed: %s", pc_errname(errno));
	if (pipe(p) != 0)
		pc_unresolved("pipe failed: %s", pc_errname(errno));
	nt_fd[1] = p[0];
	nt_fd[2] = p[1];
	nt_fd[3] = open("/dev/null", O_RDWR);
	if (nt_fd[3] < 0)
		pc_unresolved("open(/dev/null) failed: %s", pc_errname(errno));
}

/* A descriptor number that is certainly closed. */
static int closed_fd(void)
{
	int fd = open("plain", O_RDONLY);

	if (fd < 0)
		pc_unresolved("open(plain) failed: %s", pc_errname(errno));
	if (close(fd) != 0)
		pc_unresolved("close failed");
	return fd;
}


int main(void)
{
	char buf[4096];
	int fds[2], i, r;
	const char *p;

	pc_begin("XSH.ttyname.007");
	open_nonttys();
	fds[0] = -1;
	fds[1] = closed_fd();
	pc_test();
	for (i = 0; i < 2; i++) {
		errno = 0;
		p = ttyname(fds[i]);
		pc_check(p == NULL, "ttyname(%d) returned a name", fds[i]);
		pc_check(errno != 0, "ttyname(%d) failed without setting errno", fds[i]);
		r = ttyname_r(fds[i], buf, sizeof buf);
		pc_check(r > 0, "ttyname_r(%d) returned %d, expected an error number", fds[i], r);
	}
	pc_pass();
}
