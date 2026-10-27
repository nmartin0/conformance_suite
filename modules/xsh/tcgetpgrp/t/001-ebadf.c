/*
 * tcgetpgrp() of a bad descriptor.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.tcgetpgrp.001
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


/*
 * 1 if the process certainly has a controlling terminal (the pathname from
 * ctermid() can be opened). Without one, ENOTTY also applies to every call
 * here (no controlling terminal), and XSH 2.3 lets either error be returned.
 */
static int has_ctty(void)
{
	char cb[L_ctermid];
	int fd;

	ctermid(cb);
	if (cb[0] == '\0')
		return 0;
	fd = open(cb, O_RDWR | O_NOCTTY);
	if (fd < 0)
		return 0;
	close(fd);
	return 1;
}

int main(void)
{
	int c, ctty;
	pid_t r;

	pc_begin("XSH.tcgetpgrp.001");
	open_nonttys();
	c = closed_fd();
	ctty = has_ctty();
	pc_test();
	errno = 0;
	r = tcgetpgrp(-1);
	pc_check(r == -1 && (errno == EBADF || (!ctty && errno == ENOTTY)), "tcgetpgrp(-1) returned %ld, errno %s", (long)r, pc_errname(errno));
	errno = 0;
	r = tcgetpgrp(c);
	pc_check(r == -1 && (errno == EBADF || (!ctty && errno == ENOTTY)), "tcgetpgrp() of a closed descriptor returned %ld, errno %s", (long)r, pc_errname(errno));
	pc_pass();
}
