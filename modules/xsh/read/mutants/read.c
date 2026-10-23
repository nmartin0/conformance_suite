/*
 * read.c - planted faults in read() and pread(), for mutation checks
 * (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "read.<fault>" and apply only while
 * PC_PHASE is "test".
 */
#define _GNU_SOURCE 1
#include <dlfcn.h>
#include <errno.h>
#include <fcntl.h>
#include <poll.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>

typedef ssize_t (*read_fn)(int, void *, size_t);
typedef ssize_t (*pread_fn)(int, void *, size_t, off_t);

static const char *mutant(void)
{
	const char *ph = getenv("PC_PHASE");
	const char *m = getenv("PC_MUTANT");
	if (ph == NULL || strcmp(ph, "test") != 0 || m == NULL ||
	    strncmp(m, "read.", 5) != 0)
		return NULL;
	return m + 5;
}

#define IS(name) (m != NULL && strcmp(m, name) == 0)

static read_fn real_read(void)
{
	static read_fn fn;
	if (fn == NULL)
		fn = (read_fn)dlsym(RTLD_NEXT, "read");
	return fn;
}

static int kind(int fd)
{
	struct stat st;
	if (fstat(fd, &st) != 0)
		return 0;
	if (S_ISREG(st.st_mode)) return 'r';
	if (S_ISFIFO(st.st_mode)) return 'p';
	if (S_ISSOCK(st.st_mode)) return 's';
	return 'o';
}

static int nothing_ready(int fd)
{
	struct pollfd p;
	p.fd = fd;
	p.events = POLLIN;
	return poll(&p, 1, 0) == 0;
}

ssize_t read(int fd, void *buf, size_t n)
{
	read_fn f = real_read();
	const char *m = mutant();
	struct stat st0;
	int k, fl, e;
	ssize_t r;

	if (m == NULL)
		return f(fd, buf, n);
	k = kind(fd);
	fl = fcntl(fd, F_GETFL);
	if (IS("zero_moves") && n == 0) {
		(void)lseek(fd, 1, SEEK_CUR);
		return 0;
	}
	if (IS("short_by_one") && k == 'r' && n > 1)
		n--;
	if (IS("pipe_eof_eagain") && k == 'p') {
		/* Empty with no writer (hang-up, nothing to read): "try again". */
		struct pollfd p = { fd, POLLIN, 0 };
		if (poll(&p, 1, 0) == 1 && (p.revents & POLLHUP) &&
		    !(p.revents & POLLIN)) {
			errno = EAGAIN;
			return -1;
		}
	}
	if (IS("pipe_block_eagain") && (k == 'p' || k == 's') &&
	    fl != -1 && !(fl & O_NONBLOCK) && nothing_ready(fd)) {
		errno = EAGAIN;
		return -1;
	}
	if (IS("nonblock_data_eagain") && fl != -1 && (fl & O_NONBLOCK)) {
		errno = EAGAIN;	/* "would block" even with data there */
		return -1;
	}
	if (IS("nonblock_blocks") && fl != -1 && (fl & O_NONBLOCK)) {
		(void)fcntl(fd, F_SETFL, fl & ~O_NONBLOCK);
		r = f(fd, buf, n);
		e = errno;
		(void)fcntl(fd, F_SETFL, fl);
		errno = e;
		return r;
	}
	if (IS("waits_for_full") && (k == 'p' || k == 's')) {
		size_t got = 0;
		while (got < n) {
			r = f(fd, (char *)buf + got, n - got);
			if (r <= 0)
				return got ? (ssize_t)got : r;
			got += (size_t)r;
		}
		return (ssize_t)got;
	}
	if (IS("no_atime") && k == 'r')
		(void)fstat(fd, &st0);
	r = f(fd, buf, n);
	while (r == -1 && errno == EINTR && IS("eintr_restarts"))
		r = f(fd, buf, n);	/* as if SA_RESTART were always on */
	e = errno;
	if (r >= 0 && IS("eisdir_as_einval") && k == 'o' && fstat(fd, &st0) == 0 &&
	    S_ISDIR(st0.st_mode))
		fprintf(stderr, "pcmutant: cannot plant read.eisdir_as_einval here\n");
	if (r == -1) {
		if (IS("eintr_as_eagain") && e == EINTR)
			e = EAGAIN;
		if (IS("eagain_as_eintr") && e == EAGAIN)
			e = EINTR;
		if (IS("ebadf_as_einval") && e == EBADF)
			e = EINVAL;
		if (IS("wronly_ok") && e == EBADF && fl != -1)
			return 0;
		if (IS("eisdir_as_einval") && e == EISDIR)
			e = EINVAL;
		if (IS("enotconn_as_eof") && e == ENOTCONN)
			return 0;
		if (IS("econnreset_as_eof") && e == ECONNRESET)
			return 0;
		errno = e;
		return -1;
	}
	if (IS("offset_not_advanced") && k == 'r' && r > 0)
		(void)lseek(fd, -r, SEEK_CUR);
	if (IS("past_eof") && k == 'r' && (size_t)r < n) {
		memset((char *)buf + r, 'x', n - (size_t)r);
		r = (ssize_t)n;
	}
	if (IS("zeros_as_junk") && k == 'r') {
		ssize_t i;
		for (i = 0; i < r; i++)
			if (((char *)buf)[i] == 0)
				((char *)buf)[i] = 'x';
	}
	if (IS("socket_drops_last") && k == 's' && r > 1)
		r--;
	if (IS("no_atime") && k == 'r') {
		struct timespec ts[2];
		ts[0] = st0.st_atim;
		ts[1].tv_sec = 0;
		ts[1].tv_nsec = UTIME_OMIT;
		(void)futimens(fd, ts);
	}
	return r;
}

ssize_t pread(int fd, void *buf, size_t n, off_t off)
{
	static pread_fn real;
	const char *m = mutant();
	ssize_t r;
	off_t cur;

	if (real == NULL)
		real = (pread_fn)dlsym(RTLD_NEXT, "pread");
	if (m == NULL)
		return real(fd, buf, n, off);
	if (IS("pread_neg_ok") && off < 0)
		return 0;
	if (IS("pread_ignores_offset"))
		return real_read()(fd, buf, n);
	cur = lseek(fd, 0, SEEK_CUR);
	r = real(fd, buf, n, off);
	if (r == -1 && IS("pread_espipe_as_einval") && errno == ESPIPE)
		errno = EINVAL;
	if (r > 0 && IS("pread_moves_offset") && cur != -1)
		(void)lseek(fd, off + r, SEEK_SET);
	return r;
}

#ifdef __GLIBC__
ssize_t pread64(int fd, void *buf, size_t n, off_t off)
{
	return pread(fd, buf, n, off);
}
#endif
