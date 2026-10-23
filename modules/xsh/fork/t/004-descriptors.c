/*
 * The child's descriptors share the parent's open file descriptions.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fork.004
 * MUTANT    fork.child_reopens_fds
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

/*
 * Waits for the child; returns its exit status (0 = all its checks held).
 * A child that did not exit normally is UNRESOLVED.
 */
static int reap(pid_t pid)
{
	int st;

	if (waitpid(pid, &st, 0) != pid)
		pc_unresolved("waitpid failed");
	if (!WIFEXITED(st))
		pc_unresolved("the child was killed by signal %d", WIFSIGNALED(st) ? WTERMSIG(st) : 0);
	return WEXITSTATUS(st);
}

int main(void)
{
	int fd;
	pid_t r;

	pc_begin("XSH.fork.004");
	pc_make_file("f", "0123456789");
	fd = open("f", O_RDONLY);
	pc_check(fd >= 0, "cannot open f");
	pc_test();
	r = fork();
	if (r == -1)
		pc_unresolved("fork failed");
	if (r == 0) {
		if (fcntl(fd, F_GETFD) == -1)
			_exit(1);
		_exit(lseek(fd, 7, SEEK_SET) == 7 ? 0 : 2);
	}
	pc_check(reap(r) == 0, "the child does not have the descriptor, or cannot seek it");
	pc_check(lseek(fd, 0, SEEK_CUR) == 7, "the child's lseek() did not move the parent's offset");
	pc_pass();
}
