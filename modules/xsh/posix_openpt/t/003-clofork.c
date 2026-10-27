/*
 * O_CLOFORK sets close-on-fork.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.posix_openpt.003
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

int main(void)
{
	int m, fd, st;
	pid_t c;

	pc_begin("XSH.posix_openpt.003");
	pc_test();
	m = openpt_setup(O_RDWR | O_NOCTTY | O_CLOFORK);
	fd = fcntl(m, F_GETFD);
	pc_check(fd != -1 && (fd & FD_CLOFORK) != 0, "FD_CLOFORK is not set (flags %d)", fd);
	fflush(NULL);
	c = fork();
	NEED(c != -1, "fork failed: %s", pc_errname(errno));
	if (c == 0)
		_exit(fcntl(m, F_GETFD) == -1 && errno == EBADF ? 0 : 1);
	NEED(waitpid(c, &st, 0) == c, "waitpid failed");
	pc_check(WIFEXITED(st) && WEXITSTATUS(st) == 0, "the descriptor was open in the child");
	pc_pass();
}
