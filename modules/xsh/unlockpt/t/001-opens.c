/*
 * After unlockpt() the subsidiary can be opened.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.unlockpt.001
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
	const char *name;
	int m, s;

	pc_begin("XSH.unlockpt.001");
	m = openpt_setup(O_RDWR | O_NOCTTY);
	NEED(grantpt(m) == 0, "grantpt failed: %s", pc_errname(errno));
	pc_test();
	pc_check(unlockpt(m) == 0, "unlockpt() failed: %s", pc_errname(errno));
	name = ptsname(m);
	NEED(name != NULL, "ptsname failed: %s", pc_errname(errno));
	s = open(name, O_RDWR | O_NOCTTY);
	pc_check(s >= 0, "the subsidiary %s cannot be opened: %s", name, pc_errname(errno));
	pc_pass();
}
