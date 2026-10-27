/*
 * With O_NONBLOCK an empty queue gives EAGAIN at once, whatever abstime says.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mq_timedreceive.005
 */
#include "mqh.h"

int main(void)
{
	struct timespec to;
	char buf[64];
	const char *n;
	mqd_t q;
	ssize_t r;
	int e;

	pc_begin("XSH.mq_timedreceive.005");
	n = mq_name("a");
	q = mq_new(n, O_RDWR | O_NONBLOCK, 4, 64);
	alarm_handler_on();
	pc_test();
	to = abs_in(4000);
	alarm(10);
	r = mq_timedreceive(q, buf, sizeof buf, NULL, &to);
	e = errno;
	alarm(0);
	pc_check(r == -1, "mq_timedreceive on an empty queue returned %ld", (long)r);
	pc_check(e == EAGAIN, "errno is %s, not EAGAIN", pc_errname(e));
	pc_check(!reached(&to), "the call lasted until abstime");
	pc_pass();
}
