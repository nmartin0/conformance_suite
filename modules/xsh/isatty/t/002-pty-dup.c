/*
 * isatty() is 1 for a duplicate and for a second open of the subsidiary.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.isatty.002
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

static char ptsn[256];

/* A manager side; ptsn gets the subsidiary device's name. Any problem is UNRESOLVED. */
static int open_manager(void)
{
	const char *name;
	int m = posix_openpt(O_RDWR | O_NOCTTY);

	if (m < 0)
		pc_unresolved("posix_openpt failed: %s", pc_errname(errno));
	if (grantpt(m) != 0 || unlockpt(m) != 0 || (name = ptsname(m)) == NULL)
		pc_unresolved("cannot set up the pseudo-terminal: %s", pc_errname(errno));
	if (strlen(name) >= sizeof ptsn)
		pc_unresolved("the subsidiary name is too long");
	strcpy(ptsn, name);
	return m;
}

/* The subsidiary side opened without becoming a controlling terminal. */
static int open_pty(int *m)
{
	int s;

	*m = open_manager();
	s = open(ptsn, O_RDWR | O_NOCTTY);
	if (s < 0)
		pc_unresolved("cannot open %s: %s", ptsn, pc_errname(errno));
	return s;
}


int main(void)
{
	int m, s, d, o;

	pc_begin("XSH.isatty.002");
	s = open_pty(&m);
	d = dup(s);
	if (d < 0)
		pc_unresolved("dup failed: %s", pc_errname(errno));
	o = open(ptsn, O_RDONLY | O_NOCTTY);
	if (o < 0)
		pc_unresolved("second open of %s failed: %s", ptsn, pc_errname(errno));
	pc_test();
	pc_check(isatty(d) == 1, "isatty() of the dup() did not return 1");
	pc_check(isatty(o) == 1, "isatty() of the read-only open did not return 1");
	pc_pass();
}
