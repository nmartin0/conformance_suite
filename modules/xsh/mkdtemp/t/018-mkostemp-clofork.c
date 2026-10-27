/*
 * mkostemp() with O_CLOFORK sets the close-on-fork flag.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mkdtemp.018
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


int main(void)
{
#if defined(O_CLOFORK) && defined(FD_CLOFORK)
	char t[] = "fXXXXXX";
	int fd, fl, st;
	pid_t c;

	pc_begin("XSH.mkdtemp.018");
	pc_require_space();
	pc_test();
	fd = mkostemp(t, O_CLOFORK);
	pc_expect_ok(fd, "mkostemp()");
	fl = fcntl(fd, F_GETFD);
	pc_check(fl != -1, "fcntl(F_GETFD) failed");
	pc_check((fl & FD_CLOFORK) != 0, "FD_CLOFORK is not set");
	pc_check((fl & FD_CLOEXEC) == 0, "FD_CLOEXEC is set");
	fflush(NULL);
	c = fork();
	if (c == -1)
		pc_unresolved("fork failed: %s", pc_errname(errno));
	if (c == 0)
		_exit(fcntl(fd, F_GETFD) == -1 && errno == EBADF ? 0 : 1);
	if (waitpid(c, &st, 0) != c)
		pc_unresolved("waitpid failed");
	pc_check(WIFEXITED(st) && WEXITSTATUS(st) == 0, "the child still has the descriptor open");
	pc_pass();
#else
	pc_begin("XSH.mkdtemp.018");
	pc_test();
	pc_unresolved("O_CLOFORK or FD_CLOFORK is not defined by the headers");
#endif
}
