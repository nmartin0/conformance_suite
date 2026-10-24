/*
 * write.c - conforming variants of write() and send(), and of the
 * {PIPE_BUF} limit, for the variant checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; see core/pcvariant.h. Each variant says why the
 * standard allows it. A socket write is send() with no flags (XSH write(),
 * DESCRIPTION), so send() varies with write() wherever a variant concerns
 * sockets.
 */
#define _GNU_SOURCE 1
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "pcvariant.h"

/* Must equal the value in write.ewouldblock.h. */
#define VAR_EWOULDBLOCK 1001

/* The variants' {PIPE_BUF}: the least value XBD <limits.h> allows. */
#define VAR_PIPE_BUF 512

#ifndef MSG_NOSIGNAL
#define MSG_NOSIGNAL 0
#endif

/* 'p' pipe or FIFO, 's' stream socket, 'q' other socket, 'o' other. */
static int kind(int fd)
{
	struct stat st;
	int t;
	socklen_t tl = sizeof t;

	if (fstat(fd, &st) != 0)
		return 0;
	if (S_ISFIFO(st.st_mode))
		return 'p';
	if (S_ISSOCK(st.st_mode))
		return getsockopt(fd, SOL_SOCKET, SO_TYPE, &t, &tl) == 0 &&
		    t == SOCK_STREAM ? 's' : 'q';
	return 'o';
}

static int nonblocking(int fd)
{
	int fl = fcntl(fd, F_GETFL);

	return fl != -1 && (fl & O_NONBLOCK);
}

/*
 * A write or send on a stream socket, with the socket variants applied.
 * "ewouldblock": EWOULDBLOCK has a value of its own (the header half is
 * write.ewouldblock.h) and a non-blocking socket write that would wait
 * fails with it; XBD <errno.h> lets EWOULDBLOCK equal EAGAIN or not, and
 * XSH write() and send() allow either error for a socket. Pipes and FIFOs
 * keep EAGAIN, which write() requires there.
 * "peer_gone_reset" (declared by no test: it models the reading of Q-61
 * that XSH.write.026, disputed, does not take): a write on a stream socket whose peer has gone fails
 * with ECONNRESET and no SIGPIPE. Such a socket is no longer connected
 * (EPIPE, with SIGPIPE), and is not connected (ECONNRESET in write()'s
 * list, ENOTCONN in send()'s): several conditions hold, and XSH 2.3 lets
 * the implementation report any of them; SIGPIPE goes only with EPIPE.
 * The peer is taken to be gone when it has also stopped sending (a peek
 * finds end of file); a socket only shut down for writing here keeps
 * EPIPE, the one error that applies to it.
 * "unconn_econnreset": a write on a stream socket that was never
 * connected fails with ECONNRESET: XSH write() gives that error for a
 * socket that is not connected (SI-1).
 */
static ssize_t sock_out(const char *v, int fd, const void *buf, size_t n, int flags, int k,
    ssize_t (*plain)(int, const void *, size_t, int))
{
	ssize_t r;
	char c;

	if (IS("peer_gone_reset") && k == 's') {
		r = plain(fd, buf, n, flags | MSG_NOSIGNAL);
		if (r == -1 && errno == EPIPE) {
			if (recv(fd, &c, 1, MSG_PEEK | MSG_DONTWAIT) == 0) {
				errno = ECONNRESET;
				return -1;
			}
			if (!(flags & MSG_NOSIGNAL))
				(void)raise(SIGPIPE);
			errno = EPIPE;
		}
		return r;
	}
	r = plain(fd, buf, n, flags);
	if (r == -1 && IS("ewouldblock") && (k == 's' || k == 'q') && errno == EAGAIN)
		errno = VAR_EWOULDBLOCK;
	if (r == -1 && IS("unconn_econnreset") && k == 's' && errno == ENOTCONN)
		errno = ECONNRESET;
	return r;
}

static ssize_t (*real_send_fn(void))(int, const void *, size_t, int)
{
	REAL(send);
	return real_send;
}

ssize_t write(int fd, const void *buf, size_t n)
{
	const char *v = variant_of("write.");
	ssize_t r;
	int k;
	REAL(write);

	if (v == NULL)
		return real_write(fd, buf, n);
	k = kind(fd);
	if (k == 's' || k == 'q') {
		if (IS("ewouldblock") || IS("peer_gone_reset") || IS("unconn_econnreset"))
			return sock_out(v, fd, buf, n, 0, k, real_send_fn());
		return real_write(fd, buf, n);
	}
	if (k != 'p' || !nonblocking(fd))
		return real_write(fd, buf, n);
	/*
	 * "nb_pipe_min": a non-blocking pipe write of more than {PIPE_BUF}
	 * bytes moves only {PIPE_BUF} of them when it can, the least XSH
	 * write() allows (DESCRIPTION, O_NONBLOCK set: at least {PIPE_BUF}
	 * bytes once the pipe has been emptied, and some when there is room).
	 * When even that does not fit, the system's own partial transfer or
	 * EAGAIN stands.
	 */
	if (IS("nb_pipe_min") && n > PIPE_BUF) {
		r = real_write(fd, buf, PIPE_BUF);
		if (r > 0 || errno != EAGAIN)
			return r;
		return real_write(fd, buf, n);
	}
	/*
	 * "pipe_buf_512" and "pipe_buf_unset": {PIPE_BUF} is 512, the least
	 * value XBD <limits.h> allows ({_POSIX_PIPE_BUF}). The real pipes keep
	 * larger writes atomic, which the text allows. A non-blocking write
	 * of more than 512 bytes may no longer fail outright while there is
	 * room (XSH write(): with O_NONBLOCK, a write over {PIPE_BUF} moves
	 * what it can when at least one byte fits), so when the real write
	 * refuses it as a whole, it moves what fits, a byte at a time.
	 */
	if ((IS("pipe_buf_512") || IS("pipe_buf_unset")) && n > VAR_PIPE_BUF) {
		size_t done = 0;

		r = real_write(fd, buf, n);
		if (r != -1 || errno != EAGAIN)
			return r;
		while (done < n && (r = real_write(fd, (const char *)buf + done, 1)) == 1)
			done++;
		if (done > 0)
			return (ssize_t)done;
		errno = EAGAIN;
		return -1;
	}
	return real_write(fd, buf, n);
}

ssize_t send(int fd, const void *buf, size_t n, int flags)
{
	const char *v = variant_of("write.");
	REAL(send);

	if (v == NULL)
		return real_send(fd, buf, n, flags);
	return sock_out(v, fd, buf, n, flags, kind(fd), real_send);
}

/*
 * {PIPE_BUF} as fpathconf() and pathconf() report it, for "pipe_buf_512"
 * and "pipe_buf_unset" (see above); the header halves set or remove the
 * <limits.h> value to match.
 */
long fpathconf(int fd, int name)
{
	const char *v = variant_of("write.");
	long r;
	REAL(fpathconf);

	r = real_fpathconf(fd, name);
	if (name == _PC_PIPE_BUF && r > VAR_PIPE_BUF &&
	    (IS("pipe_buf_512") || IS("pipe_buf_unset")))
		r = VAR_PIPE_BUF;
	return r;
}

long pathconf(const char *path, int name)
{
	const char *v = variant_of("write.");
	long r;
	REAL(pathconf);

	r = real_pathconf(path, name);
	if (name == _PC_PIPE_BUF && r > VAR_PIPE_BUF &&
	    (IS("pipe_buf_512") || IS("pipe_buf_unset")))
		r = VAR_PIPE_BUF;
	return r;
}
