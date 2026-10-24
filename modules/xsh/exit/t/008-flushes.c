/*
 * exit() flushes open streams.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.exit.008
 * MUTANT    exit.no_flush
 * VARIANT   wait.status_encoding
 * VARIANT   exit.write_through
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

int main(void)
{
	pid_t c;
	int st;

	pc_begin("XSH.exit.008");
	pc_make_file("f", "");
	pc_make_file("o", "");
	pc_test();
	c = spawn();
	if (c == 0) {
		static char buf[4096];
		FILE *f = fopen("f", "w");
		int fd = open("o", O_WRONLY);

		if (f == NULL || setvbuf(f, buf, _IOFBF, sizeof buf) != 0 ||
		    fd == -1 || dup2(fd, 1) != 1)
			_exit(99);
		(void)close(fd);
		(void)fputs("buffered", f);
		(void)fputs("stdout", stdout);	/* a file: usually fully buffered */
		exit(0);
	}
	st = reap(c);
	pc_check(WIFEXITED(st) && WEXITSTATUS(st) == 0, "the child failed (%#x)", st);
	pc_check(pc_file_equals("f", "buffered"), "the buffered data did not reach the file");
	pc_check(pc_file_equals("o", "stdout"), "the data buffered on standard output did not reach the file");
	pc_pass();
}
