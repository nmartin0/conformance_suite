/*
 * File and descriptor functions listed as cancellation points act on a
 * pending cancellation request before they return.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.9-threads.012
 *
 * For each of open(), openat(), creat(), close(), posix_close(), read(),
 * write(), pread(), pwrite(), readv(), writev() and fcntl() with
 * F_SETLKW, a thread with cancelability enabled and a request already
 * made against it calls the function (on arguments that would let it
 * complete at once); the thread must be canceled and the call must not
 * return. See the harness for how this is arranged.
 */
#include "pccp.h"
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/uio.h>

static volatile long sink;
static int pfd[2];
static int ffd;
static int spare;

static void s_pipe_byte(void) { sink = write(pfd[1], "x", 1); }
static void s_spare(void) { spare = dup(ffd); }

static void c_open(void) { sink = open("f", O_RDONLY); }
static void c_openat(void) { sink = openat(AT_FDCWD, "f", O_RDONLY); }
static void c_creat(void) { sink = creat("g", 0600); }
static void c_close(void) { sink = close(spare); }
static void c_posix_close(void) { sink = posix_close(spare, 0); }
static void c_read(void) { char b; sink = read(pfd[0], &b, 1); }
static void c_write(void) { sink = write(pfd[1], "y", 1); }
static void c_pread(void) { char b; sink = pread(ffd, &b, 1, 0); }
static void c_pwrite(void) { sink = pwrite(ffd, "z", 1, 0); }
static void c_readv(void)
{
	char b[2];
	struct iovec v[2];

	v[0].iov_base = &b[0]; v[0].iov_len = 1;
	v[1].iov_base = &b[1]; v[1].iov_len = 1;
	sink = readv(ffd, v, 2);
}
static void c_writev(void)
{
	struct iovec v[2];

	v[0].iov_base = (void *)"a"; v[0].iov_len = 1;
	v[1].iov_base = (void *)"b"; v[1].iov_len = 1;
	sink = writev(pfd[1], v, 2);
}
static void c_setlkw(void)
{
	struct flock fl;

	memset(&fl, 0, sizeof fl);
	fl.l_type = F_WRLCK;
	fl.l_whence = SEEK_SET;
	fl.l_start = 0;
	fl.l_len = 0;
	sink = fcntl(ffd, F_SETLKW, &fl);
}

static const struct cpcase cases[] = {
	{ "open", NULL, c_open, NULL },
	{ "openat", NULL, c_openat, NULL },
	{ "creat", NULL, c_creat, NULL },
	{ "close", s_spare, c_close, NULL },
	{ "posix_close", s_spare, c_posix_close, NULL },
	{ "read", s_pipe_byte, c_read, NULL },
	{ "write", NULL, c_write, NULL },
	{ "pread", NULL, c_pread, NULL },
	{ "pwrite", NULL, c_pwrite, NULL },
	{ "readv", NULL, c_readv, NULL },
	{ "writev", NULL, c_writev, NULL },
	{ "fcntl(F_SETLKW)", NULL, c_setlkw, NULL },
};

int main(void)
{
	char bad[512];
	int n;

	pc_begin("XSH.2.9-threads.012");
	guard(250);
	pc_make_file("f", "0123456789");
	ffd = open("f", O_RDWR);
	NEED(ffd != -1, "open failed");
	NEED(pipe(pfd) == 0, "pipe failed");
	pc_test();
	n = cp_run(cases, (int)(sizeof cases / sizeof cases[0]), CP_PENDING, bad, sizeof bad);
	pc_check(n == 0, "with a cancellation request pending and cancelability enabled the thread was not canceled in: %s", bad);
	pc_pass();
}
