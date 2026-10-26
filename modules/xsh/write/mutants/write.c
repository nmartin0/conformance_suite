/*
 * write.c - planted faults in write() and pwrite(), for mutation checks
 * (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "write.<fault>" and apply only while
 * PC_PHASE is "test".
 */
#define _GNU_SOURCE 1
#include <dlfcn.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <sched.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

typedef ssize_t (*write_fn)(int, const void *, size_t);
typedef ssize_t (*pwrite_fn)(int, const void *, size_t, off_t);

static const char *mutant(void)
{
	const char *ph = getenv("PC_PHASE");
	const char *m = getenv("PC_MUTANT");
	if (ph == NULL || strcmp(ph, "test") != 0 || m == NULL ||
	    strncmp(m, "write.", 6) != 0)
		return NULL;
	return m + 6;
}

#define IS(name) (m != NULL && strcmp(m, name) == 0)

static void cannot_plant(const char *m)
{
	fprintf(stderr, "pcmutant: cannot plant write.%s here\n", m);
}

static write_fn real_write(void)
{
	static write_fn fn;
	if (fn == NULL)
		fn = (write_fn)dlsym(RTLD_NEXT, "write");
	return fn;
}

static pwrite_fn real_pwrite(void)
{
	static pwrite_fn fn;
	if (fn == NULL)
		fn = (pwrite_fn)dlsym(RTLD_NEXT, "pwrite");
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

ssize_t write(int fd, const void *buf, size_t n)
{
	write_fn f = real_write();
	const char *m = mutant();
	struct stat st0;
	int k, fl, e;
	ssize_t r;

	if (m == NULL)
		return f(fd, buf, n);
	k = kind(fd);
	fl = fcntl(fd, F_GETFL);
	if (IS("zero_writes") && n == 0 && k == 'r')
		return f(fd, "x", 1) == 1 ? 0 : -1;
	if (IS("short_by_one") && k == 'r' && n > 1)
		n--;
	if (IS("append_ignored") && k == 'r' && fl != -1 && (fl & O_APPEND)) {
		(void)fcntl(fd, F_SETFL, fl & ~O_APPEND);
		r = f(fd, buf, n);
		(void)fcntl(fd, F_SETFL, fl);
		return r;
	}
	if (IS("append_racy") && k == 'r' && fl != -1 && (fl & O_APPEND)) {
		/* Seek to the end, pause, then write there: not atomic. */
		struct timespec ts = { 0, 200000 };
		(void)fcntl(fd, F_SETFL, fl & ~O_APPEND);
		(void)lseek(fd, 0, SEEK_END);
		(void)nanosleep(&ts, NULL);
		r = f(fd, buf, n);
		e = errno;
		(void)fcntl(fd, F_SETFL, fl);
		errno = e;
		return r;
	}
	if (IS("fsize_efbig_early") && k == 'r') {
		struct rlimit rl;
		off_t cur = lseek(fd, 0, SEEK_CUR);
		if (getrlimit(RLIMIT_FSIZE, &rl) == 0 && rl.rlim_cur != RLIM_INFINITY &&
		    cur >= 0 && (rlim_t)cur < rl.rlim_cur && (rlim_t)cur + n > rl.rlim_cur) {
			errno = EFBIG;
			return -1;
		}
	}
	if (IS("sigxfsz_blocked")) {
		sigset_t s;
		sigemptyset(&s);
		sigaddset(&s, SIGXFSZ);
		(void)sigprocmask(SIG_BLOCK, &s, NULL);	/* stays blocked */
	}
	if (IS("pipe_interleave") && k == 'p' && n > 1 && n <= PIPE_BUF) {
		size_t h = n / 2;
		r = f(fd, buf, h);
		if (r != (ssize_t)h)
			return r;
		sched_yield();
		r = f(fd, (const char *)buf + h, n - h);
		return r < 0 ? r : (ssize_t)n;
	}
	if (IS("nonblock_short") && k == 'p' && fl != -1 &&
	    (fl & O_NONBLOCK) && n > 1 && n <= PIPE_BUF)
		n /= 2;
	if (IS("pipe_small_transfer") && k == 'p' && fl != -1 &&
	    (fl & O_NONBLOCK) && n > PIPE_BUF)
		n = 1;
	if (IS("nonblock_blocks") && fl != -1 && (fl & O_NONBLOCK)) {
		(void)fcntl(fd, F_SETFL, fl & ~O_NONBLOCK);
		r = f(fd, buf, n);
		e = errno;
		(void)fcntl(fd, F_SETFL, fl);
		errno = e;
		return r;
	}
	if (IS("sigpipe_suppressed")) {
		struct sigaction ign, old;
		memset(&ign, 0, sizeof ign);
		ign.sa_handler = SIG_IGN;
		(void)sigaction(SIGPIPE, &ign, &old);
		r = f(fd, buf, n);
		e = errno;
		(void)sigaction(SIGPIPE, &old, NULL);
		errno = e;
		return r;
	}
	if (IS("sigpipe_consumed")) {
		/* the signal is generated and then thrown away before write() returns */
		sigset_t s;
		struct timespec z = { 0, 0 };
		r = f(fd, buf, n);
		e = errno;
		sigemptyset(&s);
		sigaddset(&s, SIGPIPE);
		if (r == -1 && e == EPIPE)
			(void)sigtimedwait(&s, NULL, &z);
		errno = e;
		return r;
	}
	if (IS("no_mtime") && k == 'r')
		(void)fstat(fd, &st0);
	r = f(fd, buf, n);
	while (r == -1 && errno == EINTR && IS("eintr_restarts"))
		r = f(fd, buf, n);
	if (IS("eintr_restarts") && r > 0 && (size_t)r < n && k == 'p') {
		/* Keeps going after a partial write instead of returning it. */
		size_t done = (size_t)r;
		for (;;) {
			if (done >= n)
				break;
			r = f(fd, (const char *)buf + done, n - done);
			if (r > 0)
				done += (size_t)r;
			else if (!(r == -1 && errno == EINTR))
				break;
		}
		r = (ssize_t)done;
	}
	e = errno;
	if (r == -1) {
		if (IS("nonblock_partial_small") && e == EAGAIN && k == 'p' &&
		    n <= PIPE_BUF && n > 1) {
			if (f(fd, buf, 1) == 1)
				return 1;
			cannot_plant(m);	/* no room for even one byte */
			errno = EAGAIN;
			return -1;
		}
		if (IS("eintr_as_eagain") && e == EINTR)
			e = EAGAIN;
		if (IS("eagain_as_eintr") && e == EAGAIN)
			e = EINTR;
		if (IS("ebadf_as_einval") && e == EBADF)
			e = EINVAL;
		if (IS("rdonly_ok") && e == EBADF && fl != -1)
			return (ssize_t)n;
		if (IS("efbig_as_enospc") && e == EFBIG)
			e = ENOSPC;
		if (IS("sigxfsz_discarded") && e == EFBIG) {
			/* Throws away a pending SIGXFSZ, as if never generated. */
			sigset_t s;
			struct timespec z = { 0, 0 };
			sigemptyset(&s);
			sigaddset(&s, SIGXFSZ);
			(void)sigtimedwait(&s, NULL, &z);
		}
		if (IS("epipe_as_eio") && e == EPIPE)
			e = EIO;
		if (IS("notconn_ok") && (e == ENOTCONN || e == ECONNRESET))
			return (ssize_t)n;
		errno = e;
		return -1;
	}
	if (IS("offset_not_advanced") && k == 'r' && r > 0)
		(void)lseek(fd, -r, SEEK_CUR);
	if (IS("partial_as_eintr") && k == 'p' && r > 0 && (size_t)r < n) {
		errno = EINTR;
		return -1;
	}
	if (IS("socket_drops_last") && k == 's' && r > 1)
		r--;
	if (IS("no_mtime") && k == 'r' && r > 0) {
		struct timespec ts[2];
		ts[0].tv_sec = 0;
		ts[0].tv_nsec = UTIME_OMIT;
		ts[1] = st0.st_mtim;
		(void)futimens(fd, ts);
	}
	return r;
}

ssize_t pwrite(int fd, const void *buf, size_t n, off_t off)
{
	const char *m = mutant();
	ssize_t r;
	off_t cur;

	if (m == NULL)
		return real_pwrite()(fd, buf, n, off);
	if (IS("pwrite_neg_ok") && off < 0)
		return (ssize_t)n;
	if (IS("pwrite_ignores_offset"))
		return real_write()(fd, buf, n);
	if (IS("pwrite_appends")) {
		/* Writes at the end when O_APPEND is set, as some systems do. */
		int fl = fcntl(fd, F_GETFL);
		if (fl != -1 && (fl & O_APPEND)) {
			off_t end = lseek(fd, 0, SEEK_END);
			off_t was = lseek(fd, 0, SEEK_CUR);
			(void)was;
			off = end;
		}
	}
	cur = lseek(fd, 0, SEEK_CUR);
	r = real_pwrite()(fd, buf, n, off);
	if (r == -1 && IS("pwrite_espipe_as_einval") && errno == ESPIPE)
		errno = EINVAL;
	if (r > 0 && IS("pwrite_moves_offset") && cur != -1)
		(void)lseek(fd, off + r, SEEK_SET);
	return r;
}

#ifdef __GLIBC__
ssize_t pwrite64(int fd, const void *buf, size_t n, off_t off)
{
	return pwrite(fd, buf, n, off);
}
#endif
