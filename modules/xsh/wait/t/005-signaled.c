/*
 * WIFSIGNALED and WTERMSIG describe a killed child.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wait.005
 * MUTANT    wait.signaled_as_exited
 * MUTANT    wait.wrong_termsig
 * VARIANT   wait.status_encoding
 */
#include "pc.h"
#include <errno.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

int main(void)
{
	pid_t c;
	int st;

	pc_begin("XSH.wait.005");
	pc_test();
	c = fork();
	if (c == -1)
		pc_unresolved("fork failed");
	if (c == 0) {
		(void)kill(getpid(), SIGTERM);
		_exit(0);
	}
	pc_check(waitpid(c, &st, 0) == c, "waitpid failed");
	pc_check(WIFSIGNALED(st), "WIFSIGNALED is false");
	pc_check(!WIFEXITED(st), "WIFEXITED is also true");
	pc_check(WTERMSIG(st) == SIGTERM, "WTERMSIG is %d, expected SIGTERM (%d)", WTERMSIG(st), SIGTERM);
	pc_pass();
}
