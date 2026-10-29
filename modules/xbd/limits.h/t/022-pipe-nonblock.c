/*
 * A non-blocking write of {PIPE_BUF} bytes or less is all or nothing.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.limits.h.022
 */
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "pc.h"

static int wouldblock(int e)
{
	return e == EAGAIN || e == EWOULDBLOCK;
}

int main(void)
{
	int p[2], i;
	long pb, written = 0, readn = 0, n;
	char *big, chunk[512];
	static char sink[65536];
	ssize_t r;

	pc_begin("XBD.limits.h.022");
	if (pipe(p) != 0)
		pc_unresolved("pipe() failed");
	pb = fpathconf(p[1], _PC_PIPE_BUF);
	if (pb == -1)
		pc_notinuse("{PIPE_BUF} has no definite value for a pipe");
	if (pb > 1048576)
		pc_unresolved("{PIPE_BUF} is %ld, larger than this test fills", pb);
	big = malloc((size_t)pb);
	pc_check(big != NULL, "out of memory");
	memset(big, 'B', (size_t)pb);
	memset(chunk, 'C', sizeof chunk);
	if (fcntl(p[0], F_SETFL, O_NONBLOCK) == -1 || fcntl(p[1], F_SETFL, O_NONBLOCK) == -1)
		pc_unresolved("fcntl(F_SETFL, O_NONBLOCK) failed");
	pc_test();
	pc_check(pb >= _POSIX_PIPE_BUF, "fpathconf(_PC_PIPE_BUF) is %ld, below %d", pb, _POSIX_PIPE_BUF);

	/* Room in an empty pipe: the whole request is transferred. */
	r = write(p[1], big, (size_t)pb);
	pc_check(r == pb, "write() of {PIPE_BUF} = %ld bytes to an empty pipe returned %ld (%s)",
	    pb, (long)r, r == -1 ? pc_errname(errno) : "short");
	written += pb;

	/* Fill it. */
	for (n = 0; n < (1L << 18); n++) {
		r = write(p[1], chunk, sizeof chunk);
		if (r == -1)
			break;
		pc_check(r == (ssize_t)sizeof chunk, "a %zu-byte write returned %ld", sizeof chunk, (long)r);
		written += r;
	}
	pc_check(r == -1, "the pipe never filled after %ld bytes", written);
	pc_check(wouldblock(errno), "write() to a full pipe failed with %s, expected EAGAIN", pc_errname(errno));
	for (i = 0; i < (int)sizeof chunk; i++) {
		r = write(p[1], chunk, 1);
		if (r == -1)
			break;
		written += r;
	}
	pc_check(r == -1 && wouldblock(errno), "a one-byte write to a pipe that is full did not fail with EAGAIN");

	/* No room at all: nothing is written and the call fails. */
	r = write(p[1], big, (size_t)pb);
	pc_check(r == -1 && wouldblock(errno),
	    "write() of {PIPE_BUF} bytes to a full pipe returned %ld (%s), expected -1 with EAGAIN",
	    (long)r, r == -1 ? pc_errname(errno) : "wrote data");

	/* A little room, but less than {PIPE_BUF}: all or nothing. */
	n = pb - 1;
	while (n > 0) {
		r = read(p[0], sink, (size_t)n < sizeof sink ? (size_t)n : sizeof sink);
		pc_check(r > 0, "read() from a pipe holding data returned %ld", (long)r);
		n -= r;
		readn += r;
	}
	r = write(p[1], big, (size_t)pb);
	if (r == -1)
		pc_check(wouldblock(errno), "write() failed with %s, expected EAGAIN", pc_errname(errno));
	else {
		pc_check(r == pb, "write() of {PIPE_BUF} = %ld bytes into less room returned %ld", pb, (long)r);
		written += r;
	}

	/* Everything accepted comes out again. */
	while ((r = read(p[0], sink, sizeof sink)) > 0)
		readn += r;
	pc_check(r == -1 && wouldblock(errno), "draining the pipe ended with %ld (%s)", (long)r,
	    r == -1 ? pc_errname(errno) : "end of file");
	pc_check(readn == written, "%ld bytes were accepted by write() but %ld came out", written, readn);
	pc_pass();
}
