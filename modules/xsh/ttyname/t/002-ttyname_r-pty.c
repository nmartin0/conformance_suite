/*
 * ttyname_r() names the pseudo-terminal subsidiary.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.ttyname.002
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

/* path names the same file, or the same character device, as fd. */
static int names_same(const char *path, int fd)
{
	struct stat a, b;

	if (stat(path, &a) != 0 || fstat(fd, &b) != 0)
		return 0;
	if (a.st_dev == b.st_dev && a.st_ino == b.st_ino)
		return 1;
	return S_ISCHR(a.st_mode) && S_ISCHR(b.st_mode) && a.st_rdev == b.st_rdev;
}


int main(void)
{
	char buf[4096];
	int m, s, r;

	pc_begin("XSH.ttyname.002");
	s = open_pty(&m);
	pc_test();
	r = ttyname_r(s, buf, sizeof buf);
	pc_check(r == 0, "ttyname_r() returned %s", pc_errname(r));
	pc_check(buf[0] != '\0', "ttyname_r() stored an empty string");
	pc_check(names_same(buf, s), "the pathname %s does not name the terminal of the descriptor", buf);
	pc_pass();
}
