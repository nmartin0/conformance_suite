/*
 * Each call gives a separate manager.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.posix_openpt.006
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
	char n1[256], n2[256];
	const char *p;
	int m1, m2;

	pc_begin("XSH.posix_openpt.006");
	pc_test();
	m1 = openpt_setup(O_RDWR | O_NOCTTY);
	m2 = openpt_setup(O_RDWR | O_NOCTTY);
	pc_check(m1 != m2, "both calls returned descriptor %d", m1);
	NEED(grantpt(m1) == 0 && unlockpt(m1) == 0 && (p = ptsname(m1)) != NULL && strlen(p) < sizeof n1, "set-up of the first pair failed");
	strcpy(n1, p);
	NEED(grantpt(m2) == 0 && unlockpt(m2) == 0 && (p = ptsname(m2)) != NULL && strlen(p) < sizeof n2, "set-up of the second pair failed");
	strcpy(n2, p);
	pc_check(strcmp(n1, n2) != 0, "both managers have the subsidiary %s", n1);
	pc_pass();
}
