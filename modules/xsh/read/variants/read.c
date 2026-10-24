/*
 * read.c - conforming variants of read(), pread() and recv(), for the
 * variant checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; see core/pcvariant.h. Each variant says why the
 * standard allows it. A socket read is recv() with no flags (XSH read(),
 * DESCRIPTION), so recv() varies with read() wherever a variant concerns
 * sockets.
 */
#define _GNU_SOURCE 1
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "pcvariant.h"

/* Must equal the value in read.ewouldblock.h. */
#define VAR_EWOULDBLOCK 1001

/* 'p' pipe or FIFO, 's' stream socket, 'q' other socket, 'd' directory, 'o' other. */
static int kind(int fd)
{
	struct stat st;
	int t;
	socklen_t tl = sizeof t;

	if (fstat(fd, &st) != 0)
		return 0;
	if (S_ISFIFO(st.st_mode))
		return 'p';
	if (S_ISDIR(st.st_mode))
		return 'd';
	if (S_ISSOCK(st.st_mode))
		return getsockopt(fd, SOL_SOCKET, SO_TYPE, &t, &tl) == 0 &&
		    t == SOCK_STREAM ? 's' : 'q';
	return 'o';
}

/*
 * "short_stream": a read of a pipe, FIFO or stream socket that finds fewer
 * than nbyte bytes there returns only about half of them (at least one).
 * XSH read(), DESCRIPTION: the count may be less than nbyte when a pipe,
 * FIFO or special file has fewer than nbyte bytes immediately available;
 * nothing says all of the available bytes come back in one call. For a
 * stream socket, read() is recv(), and XSH recv() only says data is
 * returned as it becomes available, with no boundaries kept. A read that
 * finds nbyte or more bytes there is left full, since for pipes the text
 * gives no leave to shorten it.
 */
static size_t shorten(int fd, size_t n, int k)
{
	int avail;

	if (n <= 1 || (k != 'p' && k != 's'))
		return n;
	if (ioctl(fd, FIONREAD, &avail) != 0 || avail <= 0 || (size_t)avail >= n)
		return n;
	return ((size_t)avail + 1) / 2;
}

/*
 * "dir_readable": read() and pread() on a directory succeed and return an
 * implementation-defined image of its entries (here an opaque 32-byte
 * record, then end of file). XSH read(), ERRORS: EISDIR (XSI) is for an
 * implementation that does not allow directories to be read with read(),
 * so one that does allow it is conforming.
 */
#define DIRIMG "pc-variant-dirent\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
#define DIRIMG_LEN 32
static struct { dev_t dev; ino_t ino; off_t pos; int used; } dirpos[1024];

static ssize_t dir_read(int fd, void *buf, size_t n, off_t at, int use_at)
{
	struct stat st;
	off_t pos;
	size_t k;

	if (fd < 0 || fd >= 1024 || fstat(fd, &st) != 0) {
		errno = EBADF;
		return -1;
	}
	if (!dirpos[fd].used || dirpos[fd].dev != st.st_dev || dirpos[fd].ino != st.st_ino) {
		dirpos[fd].used = 1;
		dirpos[fd].dev = st.st_dev;
		dirpos[fd].ino = st.st_ino;
		dirpos[fd].pos = 0;
	}
	pos = use_at ? at : dirpos[fd].pos;
	if (pos < 0) {
		errno = EINVAL;
		return -1;
	}
	if (pos >= DIRIMG_LEN)
		return 0;
	k = (size_t)(DIRIMG_LEN - pos);
	if (k > n)
		k = n;
	memcpy(buf, DIRIMG + pos, k);
	if (!use_at)
		dirpos[fd].pos = pos + (off_t)k;
	return (ssize_t)k;
}

/* Error rewriting common to read() and recv() on sockets. */
static void socket_errno(const char *v, int k)
{
	/*
	 * "ewouldblock": EWOULDBLOCK is a value distinct from EAGAIN (the
	 * header half is read.ewouldblock.h), and a non-blocking socket read
	 * that would wait fails with it. XBD <errno.h> lets EWOULDBLOCK equal
	 * EAGAIN or not; XSH read() and recv() allow either error for a
	 * socket. Pipes and FIFOs keep EAGAIN, which read() requires there.
	 */
	if (IS("ewouldblock") && (k == 's' || k == 'q') && errno == EAGAIN)
		errno = VAR_EWOULDBLOCK;
	/*
	 * "reset_enotconn" (declared by no test: it models the reading of
	 * Q-61 that XSH.read.022, disputed, does not take): a read on a connection the peer reset fails with
	 * ENOTCONN. XSH read(), ERRORS lists both ECONNRESET (forcibly closed
	 * by the peer) and ENOTCONN (not connected); a reset connection is
	 * no longer connected, so both conditions hold and XSH 2.3 lets the
	 * implementation report either.
	 */
	if (IS("reset_enotconn") && k == 's' && errno == ECONNRESET)
		errno = ENOTCONN;
}

ssize_t read(int fd, void *buf, size_t n)
{
	const char *v = variant_of("read.");
	ssize_t r;
	int k;
	REAL(read);

	if (v == NULL)
		return real_read(fd, buf, n);
	k = kind(fd);
	if (IS("dir_readable") && k == 'd') {
		r = real_read(fd, buf, n);
		if (r == -1 && errno == EISDIR)
			return dir_read(fd, buf, n, 0, 0);
		return r;
	}
	if (IS("short_stream"))
		n = shorten(fd, n, k);
	r = real_read(fd, buf, n);
	if (r == -1)
		socket_errno(v, k);
	return r;
}

ssize_t pread(int fd, void *buf, size_t n, off_t off)
{
	const char *v = variant_of("read.");
	ssize_t r;
	REAL(pread);

	r = real_pread(fd, buf, n, off);
	if (r == -1 && errno == EISDIR && IS("dir_readable"))
		return dir_read(fd, buf, n, off, 1);
	return r;
}

#ifdef __GLIBC__
ssize_t pread64(int fd, void *buf, size_t n, off_t off)
{
	return pread(fd, buf, n, off);
}
#endif

ssize_t recv(int fd, void *buf, size_t n, int flags)
{
	const char *v = variant_of("read.");
	ssize_t r;
	int k;
	REAL(recv);

	if (v == NULL)
		return real_recv(fd, buf, n, flags);
	k = kind(fd);
	if (IS("short_stream") && (flags & (MSG_WAITALL | MSG_PEEK)) == 0)
		n = shorten(fd, n, k);
	r = real_recv(fd, buf, n, flags);
	if (r == -1)
		socket_errno(v, k);
	return r;
}
