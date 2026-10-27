/*
 * On a full queue mq_timedsend() fails with ETIMEDOUT only once abstime has been reached, queuing nothing.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mq_timedsend.002
 */
#include "mqh.h"

int main(void)
{
	struct timespec to;
	const char *n;
	mqd_t q;
	int r, e, late, early;

	pc_begin("XSH.mq_timedsend.002");
	n = mq_name("a");
	q = mq_new(n, O_RDWR, 2, 8);
	fill(q, 2);
	alarm_handler_on();
	pc_test();
	to = abs_in(300);
	alarm(10);
	r = mq_timedsend(q, "n", 1, 0, &to);
	e = errno;
	alarm(0);
	early = !reached(&to);
	late = since_ms(&to) > 8000;
	pc_check(r == -1, "mq_timedsend on a full queue returned %d", r);
	pc_check(e == ETIMEDOUT, "errno is %s, not ETIMEDOUT", pc_errname(e));
	pc_check(!early, "mq_timedsend returned before abstime");
	pc_check(!late, "mq_timedsend returned long after abstime");
	pc_check(curmsgs(q) == 2, "mq_curmsgs is %ld, not 2", curmsgs(q));
	pc_pass();
}
