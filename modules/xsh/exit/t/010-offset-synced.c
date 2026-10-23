/*
 * exit() sets the descriptor offset to the stream position.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.exit.010
 * MUTANT    exit.flush_no_sync
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
	{
		/* Does a stream read ahead here? If not, the test proves nothing. */
		static char pbuf[4096];
		FILE *g = fopen("f", "r");
		pc_check(g != NULL && setvbuf(g, pbuf, _IOFBF, sizeof pbuf) == 0, "fopen failed");
		pc_check(getc(g) == 'x' && getc(g) == 'x', "getc failed");
		if (lseek(fileno(g), 0, SEEK_CUR) == 2)
			pc_unresolved("the stream did not read ahead");
		(void)fclose(g);
	}
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
		exit(0);
	}
	st = reap(c);
	pc_check(WIFEXITED(st) && WEXITSTATUS(st) == 0, "the child failed (%#x)", st);
	off = lseek(fd, 0, SEEK_CUR);
	pc_check(off == 2, "the offset is %ld after exit(), expected 2", (long)off);
	pc_pass();
}
