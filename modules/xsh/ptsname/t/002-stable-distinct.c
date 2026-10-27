/*
 * ptsname() names are stable and distinct.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.ptsname.002
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
	char n1[256], n2[256];
	const char *p;
	int m1, m2;

	pc_begin("XSH.ptsname.002");
	m1 = ready_manager(n1, sizeof n1);
	m2 = ready_manager(n2, sizeof n2);
	pc_test();
	pc_check(strcmp(n1, n2) != 0, "both managers have the subsidiary %s", n1);
	p = ptsname(m1);
	pc_check(p != NULL && strcmp(p, n1) == 0, "a second ptsname() call gave another name");
	p = ptsname(m2);
	pc_check(p != NULL && strcmp(p, n2) == 0, "a second ptsname() call on the other manager gave another name");
	pc_pass();
}
