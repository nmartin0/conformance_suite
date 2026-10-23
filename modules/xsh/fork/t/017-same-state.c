/*
 * Other attributes are the same in the child.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fork.017
 * MUTANT    fork.child_umask_reset
 * MUTANT    fork.child_sigmask_cleared
 * MUTANT    fork.child_dispositions_reset
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <sys/stat.h>

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
	struct rlimit rl;
	struct stat cwd;
	sigset_t s;
	pid_t r, pg, sid;
	int code;

	pc_begin("XSH.fork.017");
	pc_make_dir("d");
	pc_check(chdir("d") == 0, "chdir failed");
	pc_check(stat(".", &cwd) == 0, "stat failed");
	(void)umask(0027);
	sigemptyset(&s);
	sigaddset(&s, SIGUSR2);
	pc_check(sigprocmask(SIG_BLOCK, &s, NULL) == 0, "sigprocmask failed");
	signal(SIGUSR1, SIG_IGN);
	pc_check(setenv("PC_FORK_TEST", "yes", 1) == 0, "setenv failed");
	pc_check(getrlimit(RLIMIT_NOFILE, &rl) == 0, "getrlimit failed");
	pg = getpgrp();
	sid = getsid(0);
	pc_test();
	r = fork();
	if (r == -1)
		pc_unresolved("fork failed");
	if (r == 0) {
		struct rlimit c;
		struct stat w;
		struct sigaction sa;
		sigset_t m;
		const char *v;
		if (umask(0) != 0027) _exit(1);
		if (stat(".", &w) != 0 || w.st_ino != cwd.st_ino || w.st_dev != cwd.st_dev) _exit(2);
		if (sigprocmask(SIG_BLOCK, NULL, &m) != 0 || sigismember(&m, SIGUSR2) != 1) _exit(3);
		if (sigaction(SIGUSR1, NULL, &sa) != 0 || sa.sa_handler != SIG_IGN) _exit(4);
		v = getenv("PC_FORK_TEST");
		if (v == NULL || strcmp(v, "yes") != 0) _exit(5);
		if (getrlimit(RLIMIT_NOFILE, &c) != 0 || c.rlim_cur != rl.rlim_cur) _exit(6);
		if (getpgrp() != pg) _exit(7);
		if (getsid(0) != sid) _exit(8);
		_exit(0);
	}
	code = reap(r);
	{
		static const char *what[] = { "", "umask", "working directory", "signal mask",
		    "ignored signal", "environment", "RLIMIT_NOFILE", "process group", "session" };
		pc_check(code == 0, "the child's %s differs from the parent's",
		    code > 0 && code < 9 ? what[code] : "state");
	}
	pc_pass();
}
