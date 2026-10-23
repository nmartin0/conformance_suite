/*
 * The child can use the parent's directory streams.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fork.006
 * MUTANT    fork.child_closes_dirs
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <dirent.h>

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
	DIR *d;
	pid_t r;

	pc_begin("XSH.fork.006");
	pc_make_dir("d");
	pc_make_file("d/x", "");
	d = opendir("d");
	pc_check(d != NULL, "opendir failed");
	pc_test();
	r = fork();
	if (r == -1)
		pc_unresolved("fork failed");
	if (r == 0) {
		struct dirent *e;
		int seen = 0;
		while ((e = readdir(d)) != NULL)
			if (strcmp(e->d_name, "x") == 0)
				seen = 1;
		_exit(seen ? 0 : 1);
	}
	pc_check(reap(r) == 0, "the child could not read the parent's directory stream");
	pc_pass();
}
