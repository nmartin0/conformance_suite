/*
 * ptsname() names a subsidiary that can be opened.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.ptsname.001
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

/* A manager made ready (granted, unlocked); the name from ptsname() is copied to nm. */
static int ready_manager(char *nm, size_t sz)
{
	const char *p;
	int m = posix_openpt(O_RDWR | O_NOCTTY);

	if (m < 0) {
		if (errno == ENOENT || errno == ENOSYS)
			pc_notinuse("no pseudo-terminal facility: %s", pc_errname(errno));
		pc_unresolved("posix_openpt failed: %s", pc_errname(errno));
	}
	if (grantpt(m) != 0 || unlockpt(m) != 0)
		pc_unresolved("cannot set up the pseudo-terminal: %s", pc_errname(errno));
	p = ptsname(m);
	if (p == NULL || strlen(p) >= sz)
		pc_unresolved("ptsname failed: %s", pc_errname(errno));
	strcpy(nm, p);
	return m;
}

int main(void)
{
	char nm[256];
	int m, s;

	pc_begin("XSH.ptsname.001");
	m = ready_manager(nm, sizeof nm);
	pc_test();
	pc_check(nm[0] != '\0', "ptsname() returned an empty string");
	s = open(nm, O_RDWR | O_NOCTTY);
	pc_check(s >= 0, "cannot open %s: %s", nm, pc_errname(errno));
	pc_check(isatty(s) == 1, "%s is not a terminal", nm);
	(void)m;
	pc_pass();
}
