/*
 * _Fork() skips the pthread_atfork() handlers.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fork.018
 * MUTANT    fork._Fork_runs_handlers
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <pthread.h>

static volatile int ran, ran_prepare, ran_parent;

static void in_child(void)
{
	ran = 1;
}

static void in_prepare(void)
{
	ran_prepare = 1;
}

static void in_parent(void)
{
	ran_parent = 1;
}

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
	pid_t r;
	int code;

	pc_begin("XSH.fork.018");
	pc_check(pthread_atfork(in_prepare, in_parent, in_child) == 0, "pthread_atfork failed");
	pc_test();
	r = _Fork();
	if (r == -1)
		pc_unresolved("_Fork failed (%s)", pc_errname(errno));
	if (r == 0)
		_exit(ran ? 1 : 0);
	code = reap(r);
	pc_check(code == 0, "_Fork() ran a pthread_atfork() child handler");
	pc_check(!ran_prepare && !ran_parent, "_Fork() ran a pthread_atfork() prepare or parent handler");
	r = fork();
	if (r == -1)
		pc_unresolved("fork failed");
	if (r == 0)
		_exit(ran ? 0 : 1);
	code = reap(r);
	pc_check(code == 0, "fork() did not run the pthread_atfork() child handler");
	pc_check(ran_prepare && ran_parent, "fork() did not run the prepare and parent handlers");
	pc_pass();
}
