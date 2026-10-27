/*
 * On an empty queue mq_timedreceive() fails with ETIMEDOUT only once abstime has been reached.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mq_timedreceive.002
 */
#include "mqh.h"

int main(void)
{
	struct timespec to;
	char buf[64];
	const char *n;
	mqd_t q;
	ssize_t r;
	int e, late, early;

	pc_begin("XSH.mq_timedreceive.002");
	n = mq_name("a");
	q = mq_new(n, O_RDWR, 4, 64);
	alarm_handler_on();
	pc_test();
	to = abs_in(300);
	alarm(10);
	r = mq_timedreceive(q, buf, sizeof buf, NULL, &to);
	e = errno;
	alarm(0);
	early = !reached(&to);
	late = since_ms(&to) > 8000;
	pc_check(r == -1, "mq_timedreceive on an empty queue returned %ld", (long)r);
	pc_check(e == ETIMEDOUT, "errno is %s, not ETIMEDOUT", pc_errname(e));
	pc_check(!early, "mq_timedreceive returned before abstime");
	pc_check(!late, "mq_timedreceive returned long after abstime");
	pc_pass();
}
