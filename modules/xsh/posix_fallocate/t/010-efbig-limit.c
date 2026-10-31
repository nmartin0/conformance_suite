/*
 * posix_fallocate() beyond RLIMIT_FSIZE gives EFBIG.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.posix_fallocate.010
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <poll.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

/* After pc_test(): a step that is not under test gives UNRESOLVED, not FAIL. */
#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

/* File "f" holding n bytes of c; a descriptor open O_RDWR. */
static int mkf(size_t n, int c)
{
	char buf[128];
	int fd;

	memset(buf, c, sizeof buf);
	fd = open("f", O_RDWR | O_CREAT | O_TRUNC, 0600);
	if (fd < 0)
		pc_unresolved("cannot create f: %s", pc_errname(errno));
	while (n > 0) {
		size_t k = n < sizeof buf ? n : sizeof buf;

		if (write(fd, buf, k) != (ssize_t)k)
			pc_unresolved("cannot fill f: %s", pc_errname(errno));
		n -= k;
	}
	return fd;
}

/* The file system may refuse the operation (ENOTSUP); then the test does not apply. */
static void check_supported(int r)
{
	if (r == ENOTSUP)
		pc_notinuse("the file system does not support posix_fallocate() (ENOTSUP)");
	if (r == ENOSPC || r == EIO || r == EINTR)
		pc_unresolved("posix_fallocate() returned %s", pc_errname(r));
}


int main(void)
{
	int p[2], fd, st, res[2], n;
	struct rlimit rl;
	pid_t c;

	pc_begin("XSH.posix_fallocate.010");
	fd = mkf(0, 'x');
	if (pipe(p) != 0)
		pc_unresolved("pipe failed: %s", pc_errname(errno));
	if (getrlimit(RLIMIT_FSIZE, &rl) != 0)
		pc_unresolved("getrlimit failed: %s", pc_errname(errno));
	if (rl.rlim_max != RLIM_INFINITY && rl.rlim_max < 1048576)
		pc_unresolved("the hard file size limit is below 1 MiB");
	pc_test();
	fflush(NULL);
	c = fork();
	if (c == -1)
		pc_unresolved("fork failed: %s", pc_errname(errno));
	if (c == 0) {
		/* The limit applies to this child only, which reports over a pipe. */
		struct sigaction sa;

		close(p[0]);
		alarm(20);
		memset(&sa, 0, sizeof sa);
		sa.sa_handler = SIG_IGN;
		sigemptyset(&sa.sa_mask);
		res[0] = -1;
		res[1] = 0;
#ifdef SIGXFSZ	/* XSI: without it there is no signal to ignore */
		if (sigaction(SIGXFSZ, &sa, NULL) == 0) {
#else
		(void)sa;
		{
			/* Outside XSI the name may be hidden but the signal still sent: block all. */
			sigset_t all;

			sigfillset(&all);
			(void)sigprocmask(SIG_BLOCK, &all, NULL);
#endif
			rl.rlim_cur = 1048576;
			if (setrlimit(RLIMIT_FSIZE, &rl) == 0)
				res[0] = posix_fallocate(fd, 0, 2097152);
		}
		if (write(p[1], res, sizeof res) != (ssize_t)sizeof res)
			_exit(2);
		_exit(0);
	}
	close(p[1]);
	n = (int)read(p[0], res, sizeof res);
	if (waitpid(c, &st, 0) != c)
		pc_unresolved("waitpid failed");
	NEED(n == (int)sizeof res, "the child did not report (status %#x)", (unsigned)st);
	NEED(res[0] != -1, "the child could not set its file size limit");
	if (res[0] == ENOTSUP)
		pc_notinuse("the file system answered ENOTSUP, which XSH 2.3 also allows here");
	NEED(res[0] != ENOSPC && res[0] != EIO, "posix_fallocate() returned %s", pc_errname(res[0]));
	pc_check(res[0] == EFBIG, "posix_fallocate() beyond the file size limit returned %s", pc_errname(res[0]));
	pc_pass();
}
