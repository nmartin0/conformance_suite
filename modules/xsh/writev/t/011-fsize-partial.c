/*
 * writev() across the file size limit writes only what fits.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.writev.011
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <sys/uio.h>

/* In the child: the limit is 10 bytes and SIGXFSZ is ignored. */
static int child(int fd)
{
	struct rlimit rl;
	struct iovec iov[2];

	(void)signal(SIGXFSZ, SIG_IGN);
	if (getrlimit(RLIMIT_FSIZE, &rl) != 0)
		return 90;
	rl.rlim_cur = 10;
	if (setrlimit(RLIMIT_FSIZE, &rl) != 0)
		return 91;
	iov[0].iov_base = "aaaaaa"; iov[0].iov_len = 6;
	iov[1].iov_base = "bbbbbb"; iov[1].iov_len = 6;
	return writev(fd, iov, 2) == 10 ? 0 : 1;
}

int main(void)
{
	pid_t kid;
	int fd, st;

	pc_begin("XSH.writev.011");
	pc_require_space();
	fd = open("f", O_WRONLY | O_CREAT, 0600);
	pc_check(fd >= 0, "cannot create f");
	pc_test();
	kid = fork();
	if (kid == -1)
		pc_unresolved("fork failed: %s", pc_errname(errno));
	if (kid == 0)
		_exit(child(fd));
	while (waitpid(kid, &st, 0) == -1)
		if (errno != EINTR)
			pc_unresolved("waitpid failed: %s", pc_errname(errno));
	pc_check(WIFEXITED(st), "the child died (status %#x)", (unsigned)st);
	if (WEXITSTATUS(st) >= 90)
		pc_unresolved("the child could not set the file size limit");
	pc_check(WEXITSTATUS(st) == 0, "writev() with room for 10 bytes did not return 10");
	pc_check(pc_file_equals("f", "aaaaaabbbb"), "the file does not hold aaaaaabbbb");
	pc_pass();
}
