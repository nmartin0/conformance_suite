/*
 * pipe.c - conforming variants of pipe() and pipe2(), for the variant
 * checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; see core/pcvariant.h. Each variant says why the
 * standard allows it.
 */
#define _GNU_SOURCE 1
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include "pcvariant.h"

/*
 * "high_read_end": of the two descriptors, the read end fildes[0] gets
 * the higher number. XSH 2.6: each allocation takes the lowest free
 * descriptor at its time, but the two allocations of pipe() may be
 * independent, so either end may get the lower one; XSH pipe() refers to
 * 2.6 for the allocation. The numbers are exchanged with dup2(), and each
 * end keeps its own FD_CLOEXEC; when no spare descriptor is free for the
 * exchange, the pipe stays as made (also a permitted order).
 */
static void read_end_high(int fd[2])
{
	int f0, f1, t;

	if (fd[0] > fd[1])
		return;
	f0 = fcntl(fd[0], F_GETFD);
	f1 = fcntl(fd[1], F_GETFD);
	t = fcntl(fd[0], F_DUPFD, fd[1] + 1);
	if (f0 == -1 || f1 == -1 || t == -1) {
		if (t != -1)
			(void)close(t);
		return;
	}
	/* fd[0]'s number takes the write end, fd[1]'s the read end. */
	if (dup2(fd[1], fd[0]) != fd[0] || dup2(t, fd[1]) != fd[1]) {
		(void)close(t);
		return;
	}
	(void)close(t);
	t = fd[0];
	fd[0] = fd[1];
	fd[1] = t;
	(void)fcntl(fd[0], F_SETFD, f0);
	(void)fcntl(fd[1], F_SETFD, f1);
}

int pipe(int fd[2])
{
	const char *v = variant_of("pipe.");
	int r;
	REAL(pipe);

	r = real_pipe(fd);
	if (r == 0 && IS("high_read_end"))
		read_end_high(fd);
	return r;
}

int pipe2(int fd[2], int flag)
{
	const char *v = variant_of("pipe.");
	int r;
	REAL(pipe2);

	r = real_pipe2(fd, flag);
	if (r == 0 && IS("high_read_end"))
		read_end_high(fd);
	return r;
}
