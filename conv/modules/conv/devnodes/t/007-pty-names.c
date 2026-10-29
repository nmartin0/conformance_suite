/*
 * The slave name posix_openpt/ptsname return names a node under /dev.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION CONV.devnodes.007
 */
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "pc.h"
#include "../../../../lib/cv.h"

int main(void)
{
	int m, s;
	const char *name, *prefix = cv_env("PTY_PREFIX");
	char path[256];
	struct stat st;

	pc_begin("CONV.devnodes.007");
	if (prefix == NULL)
		prefix = "/dev/";
	m = posix_openpt(O_RDWR | O_NOCTTY);
	pc_check(m >= 0, "posix_openpt failed: errno %d", errno);
	pc_check(grantpt(m) == 0, "grantpt failed: errno %d", errno);
	pc_check(unlockpt(m) == 0, "unlockpt failed: errno %d", errno);
	name = ptsname(m);
	pc_check(name != NULL, "ptsname failed: errno %d", errno);
	(void)snprintf(path, sizeof path, "%s", name);
	pc_test();
	pc_check(strncmp(path, prefix, strlen(prefix)) == 0, "slave name '%s' is not under %s", path, prefix);
	pc_check(stat(path, &st) == 0, "slave node %s does not exist", path);
	pc_check(S_ISCHR(st.st_mode), "slave node %s is not a character special file", path);
	s = open(path, O_RDWR | O_NOCTTY);
	pc_check(s >= 0, "cannot open slave %s: errno %d", path, errno);
	pc_check(isatty(s), "slave %s is not a terminal", path);
	(void)close(s);
	(void)close(m);
	pc_pass();
}
