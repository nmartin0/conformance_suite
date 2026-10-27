/*
 * ttyname() and ttyname_r() fail for descriptors that are not terminals.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.ttyname.006
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
	int i, r;
	const char *p;

	pc_begin("XSH.ttyname.006");
	open_nonttys();
	pc_test();
	for (i = 0; i < 4; i++) {
		errno = 0;
		p = ttyname(nt_fd[i]);
		pc_check(p == NULL, "ttyname() of the %s returned a name", nt_name[i]);
		pc_check(errno != 0, "ttyname() of the %s failed without setting errno", nt_name[i]);
		r = ttyname_r(nt_fd[i], buf, sizeof buf);
		pc_check(r > 0, "ttyname_r() of the %s returned %d, expected an error number", nt_name[i], r);
	}
	pc_pass();
}
