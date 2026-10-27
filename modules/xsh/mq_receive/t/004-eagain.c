/*
 * mq_receive() on an empty non-blocking queue fails with EAGAIN.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mq_receive.004
 */
#include "mqh.h"

int main(void)
{
	char buf[64];
	const char *n;
	mqd_t q;

	pc_begin("XSH.mq_receive.004");
	n = mq_name("a");
	q = mq_new(n, O_RDWR | O_NONBLOCK, 4, 64);
	pc_test();
	errno = 0;
	pc_check(mq_receive(q, buf, sizeof buf, NULL) == -1, "mq_receive on an empty queue returned a message");
	pc_check(errno == EAGAIN, "errno is %s, not EAGAIN", pc_errname(errno));
	pc_pass();
}
