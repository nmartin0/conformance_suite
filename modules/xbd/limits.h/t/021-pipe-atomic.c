/*
 * Writes of {PIPE_BUF} bytes or less to a pipe are not interleaved.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.limits.h.021
 */
#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include "pc.h"

#define WRITERS 4

static void writer(int fd, int id, size_t size, long count)
{
	char *b = malloc(size);
	long i;

	if (b == NULL)
		_exit(2);
	memset(b, 'A' + id, size);
	for (i = 0; i < count; i++)
		if (write(fd, b, size) != (ssize_t)size)
			_exit(1);
	_exit(0);
}

static void run(size_t size)
{
	int p[2], i, st;
	pid_t kid[WRITERS];
	long count = 2000000L / (long)size, got[WRITERS] = { 0 };
	char *blk = malloc(size);
	static char rb[65536];
	size_t have = 0, k;
	ssize_t r, j;

	pc_check(blk != NULL, "out of memory");
	if (pipe(p) != 0)
		pc_unresolved("pipe() failed");
	for (i = 0; i < WRITERS; i++) {
		kid[i] = fork();
		if (kid[i] == -1)
			pc_unresolved("fork() failed");
		if (kid[i] == 0) {
			(void)close(p[0]);
			writer(p[1], i, size, count);
		}
	}
	(void)close(p[1]);
	while ((r = read(p[0], rb, sizeof rb)) > 0) {
		for (j = 0; j < r; j++) {
			blk[have++] = rb[j];
			if (have < size)
				continue;
			have = 0;
			for (k = 1; k < size; k++)
				if (blk[k] != blk[0])
					break;
			if (k < size || blk[0] < 'A' || blk[0] >= 'A' + WRITERS)
				pc_fail("a %zu-byte write was interleaved with another (byte %zu of the record)", size, k);
			got[blk[0] - 'A']++;
		}
	}
	(void)close(p[0]);
	for (i = 0; i < WRITERS; i++)
		if (waitpid(kid[i], &st, 0) != kid[i] || !WIFEXITED(st) || WEXITSTATUS(st) != 0)
			pc_fail("a writer ended with status %#x: a write of %zu bytes was short or failed", st, size);
	pc_check(have == 0, "%zu bytes were left over after the last record", have);
	for (i = 0; i < WRITERS; i++)
		pc_check(got[i] == count, "writer %d: %ld records read, %ld written", i, got[i], count);
	free(blk);
}

int main(void)
{
	long pb;
	int p[2];

	pc_begin("XBD.limits.h.021");
	if (pipe(p) != 0)
		pc_unresolved("pipe() failed");
	pb = fpathconf(p[0], _PC_PIPE_BUF);
	(void)close(p[0]);
	(void)close(p[1]);
	if (pb == -1)
		pc_notinuse("{PIPE_BUF} has no definite value for a pipe");
	pc_test();
	pc_check(pb >= _POSIX_PIPE_BUF, "fpathconf(_PC_PIPE_BUF) is %ld, below %d", pb, _POSIX_PIPE_BUF);
	run((size_t)_POSIX_PIPE_BUF);
	if (pb > _POSIX_PIPE_BUF)
		run((size_t)(pb > 8192 ? 8192 : pb));
	pc_pass();
}
