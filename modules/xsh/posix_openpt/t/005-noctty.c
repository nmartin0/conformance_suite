/*
 * O_NOCTTY keeps the manager from becoming a controlling terminal.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.posix_openpt.005
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

/* posix_openpt() for a test of something else: no facility is NOTINUSE, resource trouble UNRESOLVED. */
static int openpt_setup(int flags)
{
	int m = posix_openpt(flags);

	if (m < 0) {
		if (errno == ENOENT || errno == ENOSYS)
			pc_notinuse("no pseudo-terminal facility: %s", pc_errname(errno));
		pc_unresolved("posix_openpt failed: %s", pc_errname(errno));
	}
	return m;
}

static int acquired(void)
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
	int p[2], st;
	pid_t c;
	char r;

	pc_begin("XSH.posix_openpt.005");
	NEED(pipe(p) == 0, "pipe failed: %s", pc_errname(errno));
	pc_test();
	fflush(NULL);
	c = fork();
	NEED(c != -1, "fork failed: %s", pc_errname(errno));
	if (c == 0) {
		int m;

		alarm(20);
		close(p[0]);
		if (setsid() == -1)
			r = 'S';
		else if ((m = posix_openpt(O_RDWR | O_NOCTTY)) < 0)
			r = 'O';
		else if (acquired())
			r = 'C';
		else
			r = 'N';
		(void)write(p[1], &r, 1);
		_exit(0);
	}
	close(p[1]);
	NEED(read(p[0], &r, 1) == 1, "the child did not report");
	NEED(waitpid(c, &st, 0) == c, "waitpid failed");
	NEED(r != 'S' && r != 'O', "set-up in the child failed (%c)", r);
	pc_check(r == 'N', "the process has a controlling terminal after posix_openpt(O_NOCTTY)");
	pc_pass();
}
