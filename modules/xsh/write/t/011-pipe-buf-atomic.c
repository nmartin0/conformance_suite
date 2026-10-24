/*
 * Pipe writes of up to {PIPE_BUF} bytes are atomic.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.write.011
 * MUTANT    write.pipe_interleave
 * VARIANT   read.short_stream
 * VARIANT   write.pipe_buf_512
 * VARIANT   write.pipe_buf_unset
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

/* {PIPE_BUF}: from <limits.h>, else from fpathconf() on the pipe. */
static long pipe_buf(int fd)
{
#ifdef PIPE_BUF
	(void)fd;
	return PIPE_BUF;
#else
	long v = fpathconf(fd, _PC_PIPE_BUF);
	if (v <= 0)
		pc_unresolved("{PIPE_BUF} is indeterminate");
	return v;
#endif
}

int main(void)
{
	long pb;
	int p[2], k, st, i;
	pid_t pid[4];
	char *buf;
	long got = 0, want;

	pc_begin("XSH.write.011");
	pc_check(pipe(p) == 0, "pipe failed");
	pb = pipe_buf(p[1]);
	want = 4L * 200 * pb;
	buf = malloc(pb);
	pc_check(buf != NULL, "out of memory");
	pc_test();
	for (k = 0; k < 4; k++) {
		pid[k] = fork();
		if (pid[k] == -1)
			pc_unresolved("fork failed");
		if (pid[k] == 0) {
			(void)close(p[0]);
			memset(buf, 'A' + k, pb);
			for (i = 0; i < 200; i++) {
				ssize_t w = write(p[1], buf, (size_t)pb);
				if (w == -1)
					_exit(1);
				if (w != pb)
					_exit(3);	/* a short blocking write */
			}
			_exit(0);
		}
	}
	(void)close(p[1]);
	for (;;) {
		long n = 0;
		ssize_t r;
		while (n < pb && (r = read(p[0], buf + n, (size_t)(pb - n))) > 0)
			n += r;
		if (n == 0)
			break;
		pc_check(n == pb, "the data ended inside a record");
		for (i = 1; i < pb; i++)
			pc_check(buf[i] == buf[0], "record %ld mixes data from two writers", got / pb);
		got += n;
	}
	for (k = 0; k < 4; k++) {
		if (waitpid(pid[k], &st, 0) != pid[k] || !WIFEXITED(st))
			pc_unresolved("a writer did not finish");
		if (WEXITSTATUS(st) == 3)
			pc_fail("a blocking write of {PIPE_BUF} bytes returned less");
		if (WEXITSTATUS(st) != 0)
			pc_unresolved("a writer failed");
	}
	pc_check(got == want, "read %ld bytes, expected %ld", got, want);
	pc_pass();
}
