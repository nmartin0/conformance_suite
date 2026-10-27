/*
 * mkostemp() with O_CLOEXEC sets the close-on-exec flag.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mkdtemp.016
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
	char t[] = "fXXXXXX";
	int fd, fl;

	pc_begin("XSH.mkdtemp.016");
	pc_require_space();
	pc_test();
	fd = mkostemp(t, O_CLOEXEC);
	pc_expect_ok(fd, "mkostemp()");
	fl = fcntl(fd, F_GETFD);
	pc_check(fl != -1, "fcntl(F_GETFD) failed");
	pc_check((fl & FD_CLOEXEC) != 0, "FD_CLOEXEC is not set");
#ifdef FD_CLOFORK
	pc_check((fl & FD_CLOFORK) == 0, "FD_CLOFORK is set");
#endif
	pc_pass();
}
