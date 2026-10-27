/*
 * posix_openpt() returns the lowest free descriptor, open read-write.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.posix_openpt.001
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
	int expect, m, fl;

	pc_begin("XSH.posix_openpt.001");
	expect = pc_lowest_free_fd();
	pc_test();
	m = openpt_setup(O_RDWR | O_NOCTTY);
	pc_check(m == expect, "posix_openpt() returned descriptor %d, the lowest free one was %d", m, expect);
	fl = fcntl(m, F_GETFL);
	pc_check(fl != -1 && (fl & O_ACCMODE) == O_RDWR, "the access mode is not read-write (flags %#x)", (unsigned)fl);
	pc_pass();
}
