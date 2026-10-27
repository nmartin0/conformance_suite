/*
 * posix_openpt() with no descriptor left.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.posix_openpt.007
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
	int first, p[2], st;
	pid_t c;
	char r;

	pc_begin("XSH.posix_openpt.007");
	first = pc_lowest_free_fd();
	NEED(pipe(p) == 0, "pipe failed: %s", pc_errname(errno));
	pc_test();
	fflush(NULL);
	c = fork();
	NEED(c != -1, "fork failed: %s", pc_errname(errno));
	if (c == 0) {
		struct rlimit rl;
		int m;

		alarm(20);
		if (getrlimit(RLIMIT_NOFILE, &rl) != 0 || (rlim_t)first > rl.rlim_max) {
			r = 'U';
		} else {
			rl.rlim_cur = (rlim_t)first;
			if (setrlimit(RLIMIT_NOFILE, &rl) != 0) {
				r = 'U';
			} else {
				errno = 0;
				m = posix_openpt(O_RDWR | O_NOCTTY);
				r = m >= 0 ? 'Y' : errno == EMFILE ? 'E' : errno == ENOENT || errno == ENOSYS ? 'A' : 'X';
			}
		}
		(void)write(p[1], &r, 1);
		_exit(0);
	}
	close(p[1]);
	NEED(read(p[0], &r, 1) == 1, "the child did not report");
	NEED(waitpid(c, &st, 0) == c, "waitpid failed");
	NEED(r != 'U', "RLIMIT_NOFILE could not be lowered");
	if (r == 'A')
		pc_notinuse("no pseudo-terminal facility");
	pc_check(r == 'E', "posix_openpt() with no free descriptor: %s", r == 'Y' ? "it succeeded" : "it failed with another error");
	pc_pass();
}
