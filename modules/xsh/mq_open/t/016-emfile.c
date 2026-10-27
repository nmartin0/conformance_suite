/*
 * With no descriptor free, mq_open() fails with EMFILE (if descriptors are file descriptors).
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mq_open.016
 */
#include "mqh.h"
#include <sys/resource.h>

int main(void)
{
	struct rlimit rl;
	const char *n;
	mqd_t q, q2;
	int e, fd;

	pc_begin("XSH.mq_open.016");
	n = mq_name("a");
	q = mq_new(n, O_RDWR, 4, 64);
	NEED(mq_close(q) == 0, "mq_close failed");
	NEED(getrlimit(RLIMIT_NOFILE, &rl) == 0, "getrlimit failed");
	if (rl.rlim_max == RLIM_INFINITY || rl.rlim_max > 64)
		rl.rlim_cur = 64;
	else
		rl.rlim_cur = rl.rlim_max;
	NEED(setrlimit(RLIMIT_NOFILE, &rl) == 0, "setrlimit failed");
	do
		fd = dup(0);
	while (fd >= 0);
	NEED(errno == EMFILE, "filling the descriptor table ended with %s, not EMFILE", pc_errname(errno));
	pc_test();
	errno = 0;
	q2 = mq_open(n, O_RDONLY);
	e = errno;
	if (q2 != BADQ)
		pc_notinuse("message queue descriptors are not limited with file descriptors here");
	pc_check(e == EMFILE, "mq_open with every descriptor in use: errno is %s, not EMFILE", pc_errname(e));
	pc_pass();
}
