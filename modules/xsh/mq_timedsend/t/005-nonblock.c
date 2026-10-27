/*
 * With O_NONBLOCK a full queue gives EAGAIN at once, whatever abstime says.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mq_timedsend.005
 */
#include "mqh.h"

int main(void)
{
	struct timespec to;
	const char *n;
	mqd_t q;
	int r, e;

	pc_begin("XSH.mq_timedsend.005");
	n = mq_name("a");
	q = mq_new(n, O_RDWR | O_NONBLOCK, 2, 8);
	fill(q, 2);
	alarm_handler_on();
	pc_test();
	to = abs_in(4000);
	alarm(10);
	r = mq_timedsend(q, "n", 1, 0, &to);
	e = errno;
	alarm(0);
	pc_check(r == -1, "mq_timedsend on a full queue returned %d", r);
	pc_check(e == EAGAIN, "errno is %s, not EAGAIN", pc_errname(e));
	pc_check(!reached(&to), "the call lasted until abstime");
	pc_pass();
}
