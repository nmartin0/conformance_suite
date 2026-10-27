/*
 * mq_timedsend() with an invalid descriptor, or one not open for writing, fails with EBADF.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mq_timedsend.010
 */
#include "mqh.h"

int main(void)
{
	struct timespec to;
	const char *n;
	mqd_t q, r;

	pc_begin("XSH.mq_timedsend.010");
	n = mq_name("a");
	q = mq_new(n, O_RDWR, 4, 8);
	r = mq_again(n, O_RDONLY);
	pc_test();
	to = abs_in(5000);
	errno = 0;
	pc_check(mq_timedsend(BADQ, "x", 1, 0, &to) == -1, "mq_timedsend on an invalid descriptor succeeded");
	pc_check(errno == EBADF, "invalid descriptor: errno is %s, not EBADF", pc_errname(errno));
	errno = 0;
	pc_check(mq_timedsend(r, "x", 1, 0, &to) == -1, "mq_timedsend on a read-only descriptor succeeded");
	pc_check(errno == EBADF, "read-only descriptor: errno is %s, not EBADF", pc_errname(errno));
	pc_check(curmsgs(q) == 0, "a message was queued despite the error");
	pc_pass();
}
