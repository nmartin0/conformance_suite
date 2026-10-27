/*
 * tcgetwinsize() of a bad descriptor.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.tcgetwinsize.003
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
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <termios.h>
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
	struct winsize w;
	long r;
	int c;

	pc_begin("XSH.tcgetwinsize.003");
	pc_make_file("plain", "x");
	c = closed_fd();
	pc_test();
	errno = 0;
	r = (long)(tcgetwinsize(-1, &w));
	pc_check(r == -1 && errno == EBADF, "tcgetwinsize(-1) returned %ld, errno %s", r, pc_errname(errno));
	errno = 0;
	r = (long)(tcgetwinsize(c, &w));
	pc_check(r == -1 && errno == EBADF, "tcgetwinsize() of a closed descriptor returned %ld, errno %s", r, pc_errname(errno));
	pc_pass();
}
