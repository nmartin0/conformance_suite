/*
 * exit() flushes streams after calling the exit handlers.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.exit.009
 * MUTANT    exit.flush_first
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
	if (WIFEXITED(st) && WEXITSTATUS(st) == 99)
		pc_unresolved("the child could not set up");
	return st;
}

static pid_t spawn(void)
{
	pid_t r = fork();

	if (r == -1)
		pc_unresolved("fork failed");
	return r;
}

static int other_fd = -1;

static void h(void)
{
	(void)write(other_fd, "H", 1);
}

int main(void)
{
	pid_t c;
	int st;

	pc_begin("XSH.exit.009");
	pc_make_file("f", "");
	pc_test();
	c = spawn();
	if (c == 0) {
		static char buf[4096];
		FILE *f = fopen("f", "a");

		other_fd = open("f", O_WRONLY | O_APPEND);
		if (f == NULL || other_fd == -1 ||
		    setvbuf(f, buf, _IOFBF, sizeof buf) != 0 || atexit(h) != 0)
			_exit(99);
		(void)fputs("S", f);
		exit(0);
	}
	st = reap(c);
	pc_check(WIFEXITED(st) && WEXITSTATUS(st) == 0, "the child failed (%#x)", st);
	pc_check(!pc_file_equals("f", "SH"), "the stream was flushed before the exit handler ran");
	pc_check(pc_file_equals("f", "HS"), "the file does not hold \"HS\"");
	pc_pass();
}
