/*
 * A clean exit gives status 0.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wait.003
 * MUTANT    wait.status_garbage
 * VARIANT   wait.status_encoding
 */
#include "pc.h"
#include <errno.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>

#include <stdio.h>

int main(int argc, char *argv[])
{
	pid_t c;
	int st, how;

	if (argc == 2 && strcmp(argv[1], "ret0") == 0)
		return 0;	/* the return-from-main child */
	pc_begin("XSH.wait.003");
	pc_check(argc >= 1 && argv[0][0] == '/', "argv[0] is not an absolute path");
	pc_test();
	for (how = 0; how < 4; how++) {
		c = fork();
		if (c == -1)
			pc_unresolved("fork failed");
		if (c == 0) {
			if (how == 0)
				_exit(0);
			if (how == 1)
				exit(0);
			if (how == 2) {
				execv(argv[0], (char *[]){ argv[0], "ret0", NULL });
				_exit(9);
			}
			_exit(1);
		}
		pc_check(waitpid(c, &st, 0) == c, "waitpid failed");
		if (how == 2 && WIFEXITED(st) && WEXITSTATUS(st) == 9)
			pc_unresolved("execv of %s failed", argv[0]);
		if (how < 3)
			pc_check(st == 0, "status %#x, expected 0 (%s)", st,
			    how == 0 ? "_exit(0)" : how == 1 ? "exit(0)" : "return 0 from main");
		else
			pc_check(st != 0, "status 0 for _exit(1)");
	}
	pc_pass();
}
