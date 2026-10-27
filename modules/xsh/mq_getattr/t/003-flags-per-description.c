/*
 * mq_flags belongs to the open description: O_NONBLOCK shows only where it was set.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mq_getattr.003
 */
#include "mqh.h"

int main(void)
{
	struct mq_attr a1, a2;
	const char *n;
	mqd_t q1, q2;

	pc_begin("XSH.mq_getattr.003");
	n = mq_name("a");
	q1 = mq_new(n, O_RDWR | O_NONBLOCK, 4, 64);
	q2 = mq_again(n, O_RDWR);
	pc_test();
	pc_check(mq_getattr(q1, &a1) == 0 && mq_getattr(q2, &a2) == 0, "mq_getattr failed");
	pc_check(a1.mq_flags & O_NONBLOCK, "the non-blocking description lacks O_NONBLOCK");
	pc_check((a2.mq_flags & O_NONBLOCK) == 0, "the blocking description has O_NONBLOCK");
	pc_check(a1.mq_maxmsg == a2.mq_maxmsg && a1.mq_msgsize == a2.mq_msgsize,
	    "the descriptors of one queue report different sizes");
	pc_pass();
}
