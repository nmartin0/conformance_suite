/*
 * _exit() does not flush streams.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH._Exit.005
 * MUTANT    _Exit.flushes
 * MUTANT    _Exit.calls_atexit
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

/* Waits for pid and returns its raw status; anything odd is UNRESOLVED. */
static int reap(pid_t pid)
{
	int st;

	if (waitpid(pid, &st, 0) != pid)
		pc_unresolved("waitpid failed");
	return st;
}

static pid_t spawn(void)
{
	pid_t r = fork();

	if (r == -1)
		pc_unresolved("fork failed");
	return r;
}

int main(void)
{
	pid_t c;
	int st;

	pc_begin("XSH._Exit.005");
	pc_make_file("f", "");
	pc_test();
	c = spawn();
	if (c == 0) {
		FILE *f = fopen("f", "w");
		static char buf[4096];
		if (f == NULL || setvbuf(f, buf, _IOFBF, sizeof buf) != 0)
			_exit(99);
		(void)fputs("unflushed", f);
		_exit(0);
	}
	st = reap(c);
	pc_check(WIFEXITED(st) && WEXITSTATUS(st) == 0, "the child failed (%#x)", st);
	pc_check(pc_file_size("f") == 0, "the buffered data reached the file: the stream was flushed");
	pc_pass();
}
