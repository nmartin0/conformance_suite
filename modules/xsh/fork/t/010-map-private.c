/*
 * MAP_PRIVATE mappings are copied at fork().
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fork.010
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
	int fd, p[2], q[2];
	pid_t r;
	int code;
	char c;

	pc_begin("XSH.fork.010");
	pc_make_file("f", "00000000");
	fd = open("f", O_RDWR);
	pc_check(fd >= 0, "cannot open f");
	m = mmap(NULL, 8, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
	pc_check(m != MAP_FAILED, "mmap failed");
	m[0] = 'A';	/* before fork: the child must see it */
	pc_check(pipe(p) == 0 && pipe(q) == 0, "pipe failed");
	pc_test();
	r = fork();
	if (r == -1)
		pc_unresolved("fork failed");
	if (r == 0) {
		if (m[0] != 'A')
			_exit(1);
		m[2] = 'C';
		(void)write(p[1], "w", 1);
		(void)read(q[0], &c, 1);	/* the parent has written m[1] */
		if (m[1] != '0')
			_exit(2);
		_exit(0);
	}
	pc_check(read(p[0], &c, 1) == 1, "the child did not report");
	m[1] = 'B';
	(void)write(q[1], "g", 1);
	code = reap(r);
	pc_check(code != 1, "the child does not see the parent's change made before fork()");
	pc_check(code != 2, "the child sees a change the parent made after fork()");
	pc_check(code == 0, "child status %d", code);
	pc_check(m[2] == '0', "the parent sees the child's change");
	pc_pass();
}
