/*
 * Pipe writes of up to {PIPE_BUF} bytes are atomic.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.write.011
 * MUTANT    write.pipe_interleave
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(void)
{
	int p[2], k, st, i;
	pid_t pid[4];
	char *buf;
	long got = 0, want = 4L * 200 * PIPE_BUF;

	pc_begin("XSH.write.011");
	pc_check(pipe(p) == 0, "pipe failed");
	buf = malloc(PIPE_BUF);
	pc_check(buf != NULL, "out of memory");
	pc_test();
	for (k = 0; k < 4; k++) {
		pid[k] = fork();
		if (pid[k] == -1)
			pc_unresolved("fork failed");
		if (pid[k] == 0) {
			(void)close(p[0]);
			memset(buf, 'A' + k, PIPE_BUF);
			for (i = 0; i < 200; i++)
				if (write(p[1], buf, PIPE_BUF) != PIPE_BUF)
					_exit(1);
			_exit(0);
		}
	}
	(void)close(p[1]);
	for (;;) {
		long n = 0;
		ssize_t r;
		while (n < PIPE_BUF && (r = read(p[0], buf + n, (size_t)(PIPE_BUF - n))) > 0)
			n += r;
		if (n == 0)
			break;
		pc_check(n == PIPE_BUF, "the data ended inside a record");
		for (i = 1; i < PIPE_BUF; i++)
			pc_check(buf[i] == buf[0], "record %ld mixes data from two writers", got / PIPE_BUF);
		got += n;
	}
	for (k = 0; k < 4; k++)
		if (waitpid(pid[k], &st, 0) != pid[k] || !WIFEXITED(st) || WEXITSTATUS(st) != 0)
			pc_unresolved("a writer failed");
	pc_check(got == want, "read %ld bytes, expected %ld", got, want);
	pc_pass();
}
