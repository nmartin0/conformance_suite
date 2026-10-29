/*
 * An exec with arguments and environment of {_POSIX_ARG_MAX} bytes works.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.limits.h.023
 */
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include "pc.h"

int main(int argc, char **argv)
{
	static char big[8192], num[16];
	char *self, *av[5], *ev[2];
	size_t used, len, i;
	pid_t kid;
	int st;

	if (argc == 4 && strcmp(argv[1], "img") == 0) {
		/* The new process image: check the long argument arrived whole. */
		len = (size_t)atol(argv[2]);
		if (strlen(argv[3]) != len)
			return 3;
		for (i = 0; i < len; i++)
			if (argv[3][i] != 'a')
				return 4;
		return 0;
	}
	pc_begin("XBD.limits.h.023");
	pc_check(argc >= 1 && argv[0][0] == '/', "argv[0] is not an absolute path");
	self = argv[0];
	/*
	 * Everything the exec carries counts: the strings with their nulls and,
	 * as some systems count them, the pointers. Stay 16 bytes inside the
	 * minimum so that either way of counting leaves it within the limit.
	 */
	used = strlen(self) + 1 + sizeof "img" + 5 + 4 + 1 + 7 * sizeof(char *);
	pc_check(used + 100 < (size_t)_POSIX_ARG_MAX - 16, "the path of this test is too long to build the arguments");
	len = (size_t)_POSIX_ARG_MAX - 16 - used;
	memset(big, 'a', len);
	big[len] = '\0';
	(void)snprintf(num, sizeof num, "%zu", len);
	av[0] = self;
	av[1] = "img";
	av[2] = num;
	av[3] = big;
	av[4] = NULL;
	ev[0] = "PC=1";
	ev[1] = NULL;
	pc_test();
	kid = fork();
	if (kid == -1)
		pc_unresolved("fork() failed");
	if (kid == 0) {
		(void)execve(self, av, ev);
		_exit(errno == E2BIG ? 97 : 98);
	}
	pc_check(waitpid(kid, &st, 0) == kid, "waitpid() failed");
	pc_check(!(WIFEXITED(st) && WEXITSTATUS(st) == 97),
	    "exec failed with E2BIG for arguments and environment within %d bytes", _POSIX_ARG_MAX);
	pc_check(!(WIFEXITED(st) && WEXITSTATUS(st) == 98), "exec failed (not E2BIG)");
	pc_check(WIFEXITED(st) && WEXITSTATUS(st) != 3 && WEXITSTATUS(st) != 4,
	    "the new image received a damaged argument (status %d)", WEXITSTATUS(st));
	pc_check(WIFEXITED(st) && WEXITSTATUS(st) == 0, "the new image ended with status %#x", st);
	pc_pass();
}
