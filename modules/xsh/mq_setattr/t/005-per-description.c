/*
 * mq_setattr() changes the open description only, not other descriptions of the queue.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mq_setattr.005
 */
#include "mqh.h"

int main(void)
{
	struct mq_attr in, a1, a2;
	const char *n;
	mqd_t q1, q2;

	pc_begin("XSH.mq_setattr.005");
	n = mq_name("a");
	q1 = mq_new(n, O_RDWR, 4, 64);
	q2 = mq_again(n, O_RDWR);
	memset(&in, 0, sizeof in);
	in.mq_flags = O_NONBLOCK;
	pc_test();
	pc_check(mq_setattr(q1, &in, NULL) == 0, "mq_setattr failed: %s", pc_errname(errno));
	pc_check(mq_getattr(q1, &a1) == 0 && mq_getattr(q2, &a2) == 0, "mq_getattr failed");
	pc_check(a1.mq_flags & O_NONBLOCK, "the changed description lacks O_NONBLOCK");
	pc_check((a2.mq_flags & O_NONBLOCK) == 0, "another description of the queue changed too");
	pc_pass();
}
