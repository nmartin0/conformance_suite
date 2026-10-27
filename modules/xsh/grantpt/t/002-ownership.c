/*
 * grantpt() sets owner and permissions of the subsidiary.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.grantpt.002
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
	struct stat sb;
	const char *name;
	int m;

	pc_begin("XSH.grantpt.002");
	m = openpt_setup(O_RDWR | O_NOCTTY);
	pc_test();
	pc_check(grantpt(m) == 0, "grantpt() failed: %s", pc_errname(errno));
	NEED(unlockpt(m) == 0, "unlockpt failed: %s", pc_errname(errno));
	name = ptsname(m);
	NEED(name != NULL, "ptsname failed: %s", pc_errname(errno));
	NEED(stat(name, &sb) == 0, "stat(%s) failed: %s", name, pc_errname(errno));
	pc_check(sb.st_uid == getuid(), "the subsidiary belongs to user %ld, not the real user %ld", (long)sb.st_uid, (long)getuid());
	pc_check((sb.st_mode & 0620) == 0620, "the permissions are %o, lacking owner read/write or group write", (unsigned)(sb.st_mode & 07777));
	pc_pass();
}
