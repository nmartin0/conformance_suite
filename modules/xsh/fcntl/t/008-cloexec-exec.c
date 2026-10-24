/*
 * FD_CLOEXEC closes a descriptor at exec; others stay open.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fcntl.008
 * MUTANT    fcntl.setfd_ignored
 * VARIANT   fcntl.ok_other
 * VARIANT   fcntl.cloexec_bit
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
	char a_s[16], b_s[16];
	int a, b, st;
	pid_t pid;

	if (argc == 4 && strcmp(argv[1], "child") == 0) {
		/* The new image: 1 if a (FD_CLOEXEC) is open, 2 if b is not. */
		a = atoi(argv[2]);
		b = atoi(argv[3]);
		if (fcntl(a, F_GETFD) != -1)
			_exit(1);
		if (fcntl(b, F_GETFD) == -1)
			_exit(2);
		_exit(0);
	}
	pc_begin("XSH.fcntl.008");
	pc_check(argc >= 1 && argv[0][0] == '/', "argv[0] is not an absolute path");
	pc_make_file("f", "x");
	a = open("f", O_RDONLY);
	b = open("f", O_RDONLY);
	pc_check(a >= 0 && b >= 0, "cannot open f");
	snprintf(a_s, sizeof a_s, "%d", a);
	snprintf(b_s, sizeof b_s, "%d", b);
	pc_test();
	pc_expect_ok(fcntl(a, F_SETFD, FD_CLOEXEC), "fcntl(F_SETFD, FD_CLOEXEC)");
	pid = fork();
	if (pid == -1)
		pc_unresolved("fork failed");
	if (pid == 0) {
		execv(argv[0], (char *[]){ argv[0], "child", a_s, b_s, NULL });
		_exit(3);
	}
	if (waitpid(pid, &st, 0) != pid || !WIFEXITED(st) || WEXITSTATUS(st) == 3)
		pc_unresolved("the exec child did not run");
	pc_check(WEXITSTATUS(st) != 1, "the FD_CLOEXEC descriptor is open after exec");
	pc_check(WEXITSTATUS(st) != 2, "the descriptor without FD_CLOEXEC was closed by exec");
	pc_check(WEXITSTATUS(st) == 0, "child exit status %d", WEXITSTATUS(st));
	pc_pass();
}
