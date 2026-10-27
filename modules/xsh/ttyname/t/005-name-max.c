/*
 * The terminal name is no longer than TTY_NAME_MAX.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.ttyname.005
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
	char buf[4096];
	long lim;
	int m, s, r;

	pc_begin("XSH.ttyname.005");
	s = open_pty(&m);
	errno = 0;
	lim = sysconf(_SC_TTY_NAME_MAX);
	if (lim == -1)
		pc_notinuse("the system reports no TTY_NAME_MAX limit");
	pc_test();
	r = ttyname_r(s, buf, sizeof buf);
	NEED(r == 0, "ttyname_r() with a large buffer returned %s", pc_errname(r));
	pc_check((long)strlen(buf) <= lim, "ttyname_r() gave a name of %zu bytes, limit %ld", strlen(buf), lim);
	pc_pass();
}
