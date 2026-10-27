/*
 * A signal interrupting a blocked mq_receive() makes it fail with EINTR.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mq_receive.007
 */
#include "mqh.h"

int main(void)
{
	char buf[64];
	const char *n;
	mqd_t q;
	int e;
	ssize_t r;

	pc_begin("XSH.mq_receive.007");
	n = mq_name("a");
	q = mq_new(n, O_RDWR, 4, 64);
	alarm_handler_on();
	pc_test();
	alarm(1);
	r = mq_receive(q, buf, sizeof buf, NULL);
	e = errno;
	alarm(0);
	pc_check(r == -1, "mq_receive returned %ld on an empty queue", (long)r);
	pc_check(e == EINTR, "errno is %s, not EINTR", pc_errname(e));
	pc_pass();
}
