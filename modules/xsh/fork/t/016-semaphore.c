/*
 * Open semaphores stay open in the child.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fork.016
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <semaphore.h>
#include <stdio.h>

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
	char name[64];
	sem_t *s;
	pid_t r;

	pc_begin("XSH.fork.016");
	snprintf(name, sizeof name, "/pcfork%ld", (long)getpid());
	s = sem_open(name, O_CREAT | O_EXCL, 0600, 0);
	if (s == SEM_FAILED)
		pc_unresolved("sem_open failed (%s)", pc_errname(errno));
	(void)sem_unlink(name);
	alarm(10);
	pc_test();
	r = fork();
	if (r == -1)
		pc_unresolved("fork failed");
	if (r == 0)
		_exit(sem_post(s) == 0 ? 0 : 1);
	pc_check(reap(r) == 0, "sem_post() in the child failed");
	pc_check(sem_trywait(s) == 0, "the child's sem_post() did not reach the parent's semaphore (%s)",
	    pc_errname(errno));
	pc_pass();
}
