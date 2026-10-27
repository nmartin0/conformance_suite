/*
 * pwrite() at or beyond the file size limit fails with EFBIG.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pwrite.012
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

/* In the child: the limit is 4096 bytes and SIGXFSZ is ignored. */
static int child(int fd)
{
	struct rlimit rl;

	(void)signal(SIGXFSZ, SIG_IGN);
	if (getrlimit(RLIMIT_FSIZE, &rl) != 0)
		return 90;
	rl.rlim_cur = 4096;
	if (setrlimit(RLIMIT_FSIZE, &rl) != 0)
		return 91;
	if (pwrite(fd, "x", 1, 8192) != -1)
		return 1;
	return errno == EFBIG ? 0 : 2;
}

int main(void)
{
	pid_t kid;
	int fd, st;

	pc_begin("XSH.pwrite.012");
	pc_make_file("f", "0123456789");
	fd = open("f", O_RDWR);
	pc_check(fd >= 0, "cannot open f");
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
	pc_check(WEXITSTATUS(st) != 1, "pwrite() beyond the file size limit succeeded");
	pc_check(WEXITSTATUS(st) == 0, "pwrite() beyond the file size limit failed with the wrong error");
	pc_check(pc_file_equals("f", "0123456789"), "the file changed");
	pc_pass();
}
