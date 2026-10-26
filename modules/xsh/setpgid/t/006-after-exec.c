/*
 * setpgid() of a child that has run an exec function fails with EACCES.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.setpgid.006
 * MUTANT    setpgid.eacces_eperm
 * MUTANT    setpgid.exec_ok
 */
#include "pc.h"
#include <errno.h>
#include <limits.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	char rs[16], hs[16], c;
	int rp[2], hp[2], st;
	pid_t kid, before, after;
	ssize_t n;

	if (argc == 4 && strcmp(argv[1], "image") == 0) {
		/* The new image: say it runs, then wait for the test to let go. */
		ssize_t w = write(atoi(argv[2]), "x", 1);

		(void)w;
		while (read(atoi(argv[3]), &c, 1) == -1 && errno == EINTR)
			;
		_exit(0);
	}
	pc_begin("XSH.setpgid.006");
	pc_check(argc >= 1 && argv[0][0] == '/', "argv[0] is not an absolute path");
	pc_check(pipe(rp) == 0 && pipe(hp) == 0, "pipe failed");
	snprintf(rs, sizeof rs, "%d", rp[1]);
	snprintf(hs, sizeof hs, "%d", hp[0]);
	kid = fork();
	pc_check(kid != -1, "fork failed");
	if (kid == 0) {
		(void)close(rp[0]);
		(void)close(hp[1]);
		execv(argv[0], (char *[]){ argv[0], "image", rs, hs, NULL });
		_exit(3);
	}
	(void)close(rp[1]);
	(void)close(hp[0]);
	while ((n = read(rp[0], &c, 1)) == -1 && errno == EINTR)
		;
	if (n != 1)
		pc_unresolved("the exec child did not start");
	before = getpgid(kid);
	pc_check(before != -1, "getpgid(child) failed: %s", pc_errname(errno));
	pc_test();
	pc_expect_err(setpgid(kid, kid), EACCES, "setpgid(<child after exec>, <its ID>)");
	after = getpgid(kid);
	pc_check(after == before, "the child's group changed from %ld to %ld", (long)before, (long)after);
	(void)close(hp[1]);
	(void)waitpid(kid, &st, 0);
	pc_pass();
}
