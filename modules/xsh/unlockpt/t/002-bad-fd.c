/*
 * unlockpt() on a descriptor that is not a manager.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.unlockpt.002
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
	long r;
	int c, i;

	pc_begin("XSH.unlockpt.002");
	open_nonttys();
	c = closed_fd();
	pc_test();
	errno = 0;
	r = unlockpt(c);
	pc_check(r == 0 || (r == -1 && errno == EBADF), "unlockpt() of a closed descriptor returned %ld, errno %s", r, pc_errname(errno));
	errno = 0;
	r = unlockpt(nt_fd[1]);
	pc_check(r == 0 || (r == -1 && (errno == EBADF || errno == EINVAL)), "unlockpt() of a pipe read end returned %ld, errno %s", r, pc_errname(errno));
	for (i = 0; i < 4; i += 3) {
		errno = 0;
		r = unlockpt(nt_fd[i]);
		pc_check(r == 0 || (r == -1 && errno == EINVAL), "unlockpt() of a %s returned %ld, errno %s", nt_name[i], r, pc_errname(errno));
	}
	pc_pass();
}
