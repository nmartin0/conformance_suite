/*
 * pipe.c - planted faults in pipe() and pipe2(), for mutation checks
 * (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "pipe.<fault>" and apply only while
 * PC_PHASE is "test".
 */
#define _GNU_SOURCE 1
#include <dlfcn.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

typedef int (*pipe_fn)(int *);
typedef int (*pipe2_fn)(int *, int);

static const char *mutant(void)
{
	const char *ph = getenv("PC_PHASE");
	const char *m = getenv("PC_MUTANT");
	if (ph == NULL || strcmp(ph, "test") != 0 || m == NULL ||
	    strncmp(m, "pipe.", 5) != 0)
		return NULL;
	return m + 5;
}

#define IS(name) (m != NULL && strcmp(m, name) == 0)

/* See ci/mutate.sh: a fault that cannot be planted here is UNCHECKED. */
static void cannot_plant(const char *m)
{
	fprintf(stderr, "pcmutant: cannot plant pipe.%s here\n", m);
}

static void move_up(int *fd)
{
	int n = fcntl(*fd, F_DUPFD, *fd + 3);
	if (n >= 0) {
		(void)close(*fd);
		*fd = n;
	}
}

/* Faults common to pipe() and pipe2(), on a successful result. */
static void after(const char *m, int *fd)
{
	/* On systems with bidirectional pipes (FreeBSD) swapping is invisible. */
	if (IS("swapped")) {
		int t = fd[0];
		fd[0] = fd[1];
		fd[1] = t;
	}
	if (IS("not_lowest")) {
		move_up(&fd[0]);
		move_up(&fd[1]);
	}
	if (IS("cloexec_set")) {
		(void)fcntl(fd[0], F_SETFD, FD_CLOEXEC);
		(void)fcntl(fd[1], F_SETFD, FD_CLOEXEC);
	}
	if (IS("nonblock_set"))
		(void)fcntl(fd[1], F_SETFL, O_NONBLOCK);
	if (IS("wrong_owner") &&
	    fchown(fd[0], geteuid() + 1, (gid_t)-1) != 0) {
		/* Cannot change it here: report the pipe as unusable. */
		(void)close(fd[0]);
		fd[0] = -1;
	}
	if (IS("old_times")) {
		struct timespec ts[2] = { { 1, 0 }, { 1, 0 } };
		if (futimens(fd[0], ts) != 0)
			cannot_plant(m);	/* FreeBSD, OpenBSD: not on pipes */
	}
}

static int failed(const char *m, int *fd, int e)
{
	if (IS("emfile_as_enfile") && e == EMFILE)
		e = ENFILE;
	if (IS("emfile_scribbles"))
		fd[0] = fd[1] = 0;
	if (IS("emfile_leaks") && e == EMFILE)
		(void)open("/dev/null", O_RDONLY);	/* keeps the one free slot */
	errno = e;
	return -1;
}

int pipe(int fd[2])
{
	static pipe_fn real;
	const char *m = mutant();
	int r;

	if (real == NULL)
		real = (pipe_fn)dlsym(RTLD_NEXT, "pipe");
	r = real(fd);
	if (r != 0)
		return failed(m, fd, errno);
	after(m, fd);
	return 0;
}

int pipe2(int fd[2], int flag)
{
	static pipe2_fn real;
	const char *m = mutant();
	int r;

	if (real == NULL)
		real = (pipe2_fn)dlsym(RTLD_NEXT, "pipe2");
	if (IS("p2_flags_ignored"))
		flag = 0;
	if (IS("p2_nonblock_ignored"))
		flag &= ~O_NONBLOCK;
	r = real(fd, flag);
	if (r != 0)
		return failed(m, fd, errno);
	if (IS("p2_cloexec_one_end") && (flag & O_CLOEXEC))
		(void)fcntl(fd[1], F_SETFD, 0);
	if (IS("p2_cloexec_always"))
		(void)fcntl(fd[0], F_SETFD, FD_CLOEXEC);
	after(m, fd);
	return 0;
}
