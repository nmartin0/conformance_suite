/*
 * dup.c - planted faults in dup(), dup2() and dup3(), for mutation checks
 * (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library. Faults apply only while PC_PHASE is "test", and are
 * named "dup.<fault>" (one library serves all three functions).
 */
#define _GNU_SOURCE 1
#include <dlfcn.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef int (*dup_fn)(int);
typedef int (*dup2_fn)(int, int);
typedef int (*dup3_fn)(int, int, int);

static const char *mutant(void)
{
	const char *ph = getenv("PC_PHASE");
	const char *m = getenv("PC_MUTANT");
	if (ph == NULL || strcmp(ph, "test") != 0 || m == NULL ||
	    strncmp(m, "dup.", 4) != 0)
		return NULL;
	return m + 4;
}

#define IS(name) (m != NULL && strcmp(m, name) == 0)

/*
 * A fault that cannot be planted on this system says so; ci/mutate.sh
 * then reports the mutant UNCHECKED instead of MISSED.
 */
static void cannot_plant(const char *m)
{
	fprintf(stderr, "pcmutant: cannot plant dup.%s here\n", m);
}

/* A new open file description for the same file (Linux /proc). */
static int reopen(int fd)
{
	char path[64];
	int fl = fcntl(fd, F_GETFL);
	snprintf(path, sizeof path, "/proc/self/fd/%d", fd);
	if (fl == -1)
		return -1;
	return open(path, fl & (O_ACCMODE | O_APPEND));
}

static void copy_cloexec(int from, int to)
{
	int f = fcntl(from, F_GETFD);
	if (f != -1)
		(void)fcntl(to, F_SETFD, f);
}

int dup(int fd)
{
	static dup_fn real;
	const char *m = mutant();
	int r;

	if (real == NULL)
		real = (dup_fn)dlsym(RTLD_NEXT, "dup");
	if (IS("fresh_description")) {
		r = reopen(fd);
		if (r >= 0)
			return r;
		cannot_plant(m);
	}
	r = real(fd);
	if (r < 0) {
		if (IS("ebadf_as_einval") && errno == EBADF)
			errno = EINVAL;
		if (IS("emfile_as_enfile") && errno == EMFILE)
			errno = ENFILE;
		return r;
	}
	if (IS("not_lowest")) {
		int n = fcntl(r, F_DUPFD, r + 1);
		if (n >= 0) {
			(void)close(r);
			r = n;
		}
	}
	if (IS("keeps_cloexec"))
		copy_cloexec(fd, r);
	return r;
}

int dup2(int fd, int fd2)
{
	static dup2_fn real;
	const char *m = mutant();
	int r;

	if (real == NULL)
		real = (dup2_fn)dlsym(RTLD_NEXT, "dup2");
	if (IS("dup2_picks_lowest"))
		return fcntl(fd, F_DUPFD, 0);
	if (IS("dup2_same_closes") && fd == fd2) {
		(void)close(fd);
		return fd2;
	}
	if (IS("dup2_bad_src_closes") && fcntl(fd, F_GETFD) == -1)
		(void)close(fd2);
	if (IS("dup2_range_einval") && fd2 < 0) {
		errno = EINVAL;
		return -1;
	}
	if (IS("dup2_max_ok") && fd2 >= 0 && fd2 >= sysconf(_SC_OPEN_MAX) &&
	    fcntl(fd, F_GETFD) != -1)
		return fd2;	/* claims success for an out-of-range target */
	if (IS("dup2_leaks_old") && fd != fd2 && fcntl(fd2, F_GETFD) != -1)
		(void)fcntl(fd2, F_DUPFD, 100);	/* keeps the old description */
	if (IS("dup2_fresh_description") && fd != fd2) {
		int n = reopen(fd);
		if (n >= 0) {
			r = real(n, fd2);
			(void)close(n);
			return r;
		}
		cannot_plant(m);
	}
	r = real(fd, fd2);
	if (r < 0) {
		if (IS("ebadf_as_einval") && errno == EBADF)
			errno = EINVAL;
		return r;
	}
	if (IS("dup2_keeps_cloexec") && fd != fd2)
		copy_cloexec(fd, fd2);
	if (IS("dup2_same_clears_cloexec") && fd == fd2)
		(void)fcntl(fd2, F_SETFD, 0);
	return r;
}

int dup3(int fd, int fd2, int flag)
{
	static dup3_fn real;
	const char *m = mutant();
	int r;

	if (real == NULL)
		real = (dup3_fn)dlsym(RTLD_NEXT, "dup3");
	if (IS("dup3_same_ok") && fd == fd2 && fcntl(fd, F_GETFD) != -1)
		return fd2;
	if (IS("dup3_bad_src_closes") && fcntl(fd, F_GETFD) == -1)
		(void)close(fd2);
	if (IS("dup2_leaks_old") && fd != fd2 && fcntl(fd2, F_GETFD) != -1)
		(void)fcntl(fd2, F_DUPFD, 100);
	if (IS("dup3_cloexec_ignored"))
		flag &= ~O_CLOEXEC;
	if (IS("dup3_as_dup"))
		return fcntl(fd, flag & O_CLOEXEC ? F_DUPFD_CLOEXEC : F_DUPFD, 0);
	r = real(fd, fd2, flag);
	if (r < 0 && IS("ebadf_as_einval") && errno == EBADF)
		errno = EINVAL;
	if (r >= 0 && IS("dup3_keeps_cloexec"))
		copy_cloexec(fd, fd2);
	return r;
}
