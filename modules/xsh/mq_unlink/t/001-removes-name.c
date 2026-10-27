/*
 * mq_unlink() returns 0 and the name is gone at once, even while a descriptor is open.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mq_unlink.001
 */
#include "mqh.h"

/* mq_unlink may, but need not, wait for the last close: an alarm ends such a wait. */
static int unlink_open(const char *n)
{
	int r;

	alarm_handler_on();
	alarm(3);
	r = mq_unlink(n);
	if (r == -1 && errno == EINTR)
		pc_unresolved("mq_unlink waited for the last close of the queue");
	alarm(0);
	return r;
}

int main(void)
{
	const char *n;
	mqd_t q, q2;

	pc_begin("XSH.mq_unlink.001");
	n = mq_name("a");
	q = mq_new(n, O_RDWR, 4, 64);
	pc_test();
	pc_check(unlink_open(n) == 0, "mq_unlink failed: %s", pc_errname(errno));
	errno = 0;
	q2 = mq_open(n, O_RDWR);
	pc_check(q2 == BADQ, "the name still opens after mq_unlink");
	pc_check(errno == ENOENT, "errno is %s, not ENOENT", pc_errname(errno));
	(void)q;
	pc_pass();
}
