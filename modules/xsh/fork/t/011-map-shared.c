/*
 * MAP_SHARED mappings are shared with the child.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fork.011
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/mman.h>

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
	char *m;
	int fd;
	pid_t r;

	pc_begin("XSH.fork.011");
	pc_make_file("f", "00000000");
	fd = open("f", O_RDWR);
	pc_check(fd >= 0, "cannot open f");
	m = mmap(NULL, 8, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	pc_check(m != MAP_FAILED, "mmap failed");
	pc_test();
	r = fork();
	if (r == -1)
		pc_unresolved("fork failed");
	if (r == 0) {
		m[3] = 'S';
		_exit(0);
	}
	(void)reap(r);
	pc_check(m[3] == 'S', "the parent does not see the child's change to a shared mapping");
	pc_pass();
}
