/*
 * pwrite() across the file size limit writes only what fits.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pwrite.013
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
	char buf[100];

#ifdef SIGXFSZ	/* XSI: without it there is no signal to ignore */
	(void)signal(SIGXFSZ, SIG_IGN);
#else
	{
		/* Outside XSI the name may be hidden but the signal still sent: block all. */
		sigset_t all;

		sigfillset(&all);
		(void)sigprocmask(SIG_BLOCK, &all, NULL);
	}
#endif
	if (getrlimit(RLIMIT_FSIZE, &rl) != 0)
		return 90;
	rl.rlim_cur = 4096;
	if (setrlimit(RLIMIT_FSIZE, &rl) != 0)
		return 91;
	memset(buf, 'x', sizeof buf);
	return pwrite(fd, buf, sizeof buf, 4050) == 46 ? 0 : 1;
}

int main(void)
{
	pid_t kid;
	int fd, st;

	pc_begin("XSH.pwrite.013");
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
	pc_check(WEXITSTATUS(st) == 0, "pwrite() of 100 bytes with room for 46 did not return 46");
	pc_check(pc_file_size("f") == 4096, "the length is %ld, not 4096", pc_file_size("f"));
	pc_pass();
}
