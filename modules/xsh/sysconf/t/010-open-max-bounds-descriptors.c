/*
 * Every descriptor the system assigns is below sysconf(_SC_OPEN_MAX),
 * before and after the RLIMIT_NOFILE soft limit is raised.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sysconf.010
 * MUTANT    sysconf.open_max_cached
 * MUTANT    sysconf.open_max_minus_one
 * VARIANT   sysconf.open_max_hard
 * VARIANT   sysconf.errno_on_success
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <sys/resource.h>
#include <unistd.h>

static int fds[256];
static int nfds;

/* Open f until EMFILE; the highest descriptor assigned. */
static int fill(void)
{
	int fd, high = -1;

	while (nfds < 256) {
		fd = open("f", O_RDONLY);
		if (fd < 0) {
			if (errno != EMFILE)
				pc_unresolved("open failed with %s, not EMFILE", pc_errname(errno));
			return high;
		}
		fds[nfds++] = fd;
		if (fd > high)
			high = fd;
	}
	pc_unresolved("more than 256 descriptors opened under a soft limit of 128");
}

int main(void)
{
	struct rlimit rl;
	long m1, m2;
	int h1, h2;

	pc_begin("XSH.sysconf.010");
	pc_make_file("f", "x");
	pc_check(getrlimit(RLIMIT_NOFILE, &rl) == 0, "getrlimit failed: %s", pc_errname(errno));
	if (rl.rlim_max != RLIM_INFINITY && rl.rlim_max < 128)
		pc_unresolved("the RLIMIT_NOFILE hard limit is below 128");
	rl.rlim_cur = 64;
	pc_check(setrlimit(RLIMIT_NOFILE, &rl) == 0, "setrlimit failed: %s", pc_errname(errno));
	pc_test();
	m1 = sysconf(_SC_OPEN_MAX);
	h1 = fill();
	pc_check(m1 == -1 || h1 < m1, "with RLIMIT_NOFILE at 64, sysconf(_SC_OPEN_MAX) is %ld but open() assigned %d",
		 m1, h1);
	rl.rlim_cur = 128;
	if (setrlimit(RLIMIT_NOFILE, &rl) != 0)
		pc_unresolved("setrlimit failed: %s", pc_errname(errno));
	m2 = sysconf(_SC_OPEN_MAX);
	h2 = fill();
	pc_check(h2 > h1, "no descriptor above %d was assigned after RLIMIT_NOFILE was raised to 128", h1);
	pc_check(m2 == -1 || h2 < m2,
		 "after RLIMIT_NOFILE was raised to 128, sysconf(_SC_OPEN_MAX) is %ld but open() assigned %d", m2, h2);
	pc_pass();
}
