/*
 * exit() sets the descriptor offset to the stream position.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.exit.010
 * MUTANT    exit.no_flush
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
	char data[8192];
	pid_t c;
	int fd, st;
	off_t off;

	pc_begin("XSH.exit.010");
	memset(data, 'x', sizeof data);
	data[sizeof data - 1] = '\0';
	pc_make_file("f", data);
	fd = open("f", O_RDONLY);
	pc_check(fd != -1, "open failed");
	pc_test();
	c = spawn();
	if (c == 0) {
		static char buf[4096];
		FILE *f = fdopen(fd, "r");

		if (f == NULL || setvbuf(f, buf, _IOFBF, sizeof buf) != 0)
			_exit(99);
		if (getc(f) != 'x' || getc(f) != 'x')
			_exit(99);
		if (lseek(fd, 0, SEEK_CUR) == 2)
			_exit(97);	/* nothing buffered: the test proves nothing */
		exit(0);
	}
	st = reap(c);
	if (WIFEXITED(st) && WEXITSTATUS(st) == 97)
		pc_unresolved("the stream did not read ahead");
	pc_check(WIFEXITED(st) && WEXITSTATUS(st) == 0, "the child failed (%#x)", st);
	off = lseek(fd, 0, SEEK_CUR);
	pc_check(off == 2, "the offset is %ld after exit(), expected 2", (long)off);
	pc_pass();
}
