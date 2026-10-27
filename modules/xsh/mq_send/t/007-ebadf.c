/*
 * mq_send() with a descriptor that is invalid, or not open for writing, fails with EBADF.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mq_send.007
 */
#include "mqh.h"

int main(void)
{
	const char *n;
	mqd_t q, r;

	pc_begin("XSH.mq_send.007");
	n = mq_name("a");
	q = mq_new(n, O_RDWR, 4, 8);
	r = mq_again(n, O_RDONLY);
	pc_test();
	errno = 0;
	pc_check(mq_send(BADQ, "x", 1, 0) == -1, "mq_send on an invalid descriptor succeeded");
	pc_check(errno == EBADF, "invalid descriptor: errno is %s, not EBADF", pc_errname(errno));
	errno = 0;
	pc_check(mq_send(r, "x", 1, 0) == -1, "mq_send on a read-only descriptor succeeded");
	pc_check(errno == EBADF, "read-only descriptor: errno is %s, not EBADF", pc_errname(errno));
	pc_check(curmsgs(q) == 0, "a message was queued despite the error");
	pc_pass();
}
