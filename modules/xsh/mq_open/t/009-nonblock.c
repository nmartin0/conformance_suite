/*
 * O_NONBLOCK in oflag makes the description non-blocking.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mq_open.009
 */
#include "mqh.h"

int main(void)
{
	struct mq_attr at;
	char buf[64];
	const char *n;
	mqd_t q1, q2;

	pc_begin("XSH.mq_open.009");
	n = mq_name("a");
	q1 = mq_new(n, O_RDWR | O_NONBLOCK, 4, 64);
	q2 = mq_again(n, O_RDWR);
	pc_test();
	pc_check(mq_getattr(q1, &at) == 0, "mq_getattr failed");
	pc_check(at.mq_flags & O_NONBLOCK, "mq_flags lacks O_NONBLOCK");
	errno = 0;
	pc_check(mq_receive(q1, buf, sizeof buf, NULL) == -1 && errno == EAGAIN,
	    "receive on an empty non-blocking queue: errno %s, not EAGAIN", pc_errname(errno));
	pc_check(mq_getattr(q2, &at) == 0, "mq_getattr failed");
	pc_check((at.mq_flags & O_NONBLOCK) == 0, "a descriptor opened without O_NONBLOCK has it");
	pc_pass();
}
