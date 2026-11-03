/*
 * struct iovec elements carry scatter/gather I/O: a writev() gathers
 * several memory regions into one stream and a readv() scatters a
 * stream over several regions, each by iov_base and iov_len.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sys_uio.h.004
 */
#include "pc.h"
#include <string.h>
#include <sys/uio.h>
#include <unistd.h>

/* Advance an array of elements past n transferred bytes. */
static void advance(struct iovec **v, int *cnt, size_t n)
{
	while (*cnt > 0 && n >= (*v)->iov_len) {
		n -= (*v)->iov_len;
		(*v)++;
		(*cnt)--;
	}
	if (*cnt > 0) {
		(*v)->iov_base = (char *)(*v)->iov_base + n;
		(*v)->iov_len -= n;
	}
}

int main(void)
{
	int fd[2];
	char a[] = "gather", b[] = "-", c[] = "scatter";	/* 6 + 1 + 7 = 14 bytes */
	char x[5], y[2], z[7];
	struct iovec out[3], in[3], *v;
	int cnt;
	ssize_t r;

	pc_begin("XBD.sys_uio.h.004");
	pc_check(pipe(fd) == 0, "pipe() failed");
	out[0].iov_base = a; out[0].iov_len = 6;
	out[1].iov_base = b; out[1].iov_len = 1;
	out[2].iov_base = c; out[2].iov_len = 7;
	in[0].iov_base = x; in[0].iov_len = sizeof x;
	in[1].iov_base = y; in[1].iov_len = sizeof y;
	in[2].iov_base = z; in[2].iov_len = sizeof z;
	pc_test();

	/* gather, then read back as a single region */
	v = out; cnt = 3;
	while (cnt > 0) {
		r = writev(fd[1], v, cnt);
		pc_check(r > 0, "writev() returned %ld", (long)r);
		advance(&v, &cnt, (size_t)r);
	}
	{
		char got[14];
		size_t have = 0;

		while (have < sizeof got) {
			r = read(fd[0], got + have, sizeof got - have);
			pc_check(r > 0, "read() returned %ld after %lu bytes", (long)r, (unsigned long)have);
			have += (size_t)r;
		}
		pc_check(memcmp(got, "gather-scatter", 14) == 0,
		    "writev() did not put the three regions into the stream in order");
	}

	/* write as a single region, then scatter */
	{
		size_t sent = 0;

		while (sent < 14) {
			r = write(fd[1], "gather-scatter" + sent, 14 - sent);
			pc_check(r > 0, "write() returned %ld", (long)r);
			sent += (size_t)r;
		}
	}
	v = in; cnt = 3;
	while (cnt > 0) {
		r = readv(fd[0], v, cnt);
		pc_check(r > 0, "readv() returned %ld", (long)r);
		advance(&v, &cnt, (size_t)r);
	}
	pc_check(memcmp(x, "gathe", 5) == 0, "first region of readv() holds the wrong bytes");
	pc_check(memcmp(y, "r-", 2) == 0, "second region of readv() holds the wrong bytes");
	pc_check(memcmp(z, "scatter", 7) == 0, "third region of readv() holds the wrong bytes");
	close(fd[0]);
	close(fd[1]);
	pc_pass();
}
