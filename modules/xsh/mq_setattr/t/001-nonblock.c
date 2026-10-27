/*
 * mq_setattr() sets and clears O_NONBLOCK, and receiving follows it.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mq_setattr.001
 */
#include "mqh.h"

int main(void)
{
	struct mq_attr in, now;
	struct timespec to;
	char buf[64];
	const char *n;
	mqd_t q;

	pc_begin("XSH.mq_setattr.001");
	n = mq_name("a");
	q = mq_new(n, O_RDWR, 4, 64);
	memset(&in, 0, sizeof in);
	in.mq_flags = O_NONBLOCK;
	pc_test();
	pc_check(mq_setattr(q, &in, NULL) == 0, "mq_setattr(O_NONBLOCK) failed: %s", pc_errname(errno));
	pc_check(mq_getattr(q, &now) == 0, "mq_getattr failed");
	pc_check(now.mq_flags & O_NONBLOCK, "O_NONBLOCK is not set");
	errno = 0;
	pc_check(mq_receive(q, buf, sizeof buf, NULL) == -1 && errno == EAGAIN,
	    "receive on an empty queue after setting O_NONBLOCK: errno %s, not EAGAIN", pc_errname(errno));
	in.mq_flags = 0;
	pc_check(mq_setattr(q, &in, NULL) == 0, "mq_setattr(0) failed: %s", pc_errname(errno));
	pc_check(mq_getattr(q, &now) == 0, "mq_getattr failed");
	pc_check((now.mq_flags & O_NONBLOCK) == 0, "O_NONBLOCK is still set");
	to = abs_in(200);
	errno = 0;
	pc_check(mq_timedreceive(q, buf, sizeof buf, NULL, &to) == -1 && errno == ETIMEDOUT,
	    "receive on an empty blocking queue: errno %s, not ETIMEDOUT", pc_errname(errno));
	pc_pass();
}
