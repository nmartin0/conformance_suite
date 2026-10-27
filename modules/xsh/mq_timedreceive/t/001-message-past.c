/*
 * With a message queued, mq_timedreceive() succeeds even if abstime has long passed.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mq_timedreceive.001
 */
#include "mqh.h"

int main(void)
{
	struct timespec past = { 1, 0 };
	char buf[64];
	const char *n;
	unsigned p = 99;
	mqd_t q;

	pc_begin("XSH.mq_timedreceive.001");
	n = mq_name("a");
	q = mq_new(n, O_RDWR, 4, 64);
	NEED(mq_send(q, "hello", 5, 4) == 0, "mq_send failed");
	pc_test();
	pc_check(mq_timedreceive(q, buf, sizeof buf, &p, &past) == 5, "mq_timedreceive with a message and a past time failed: %s", pc_errname(errno));
	pc_check(memcmp(buf, "hello", 5) == 0, "the content differs");
	pc_check(p == 4, "the priority is %u, not 4", p);
	pc_check(curmsgs(q) == 0, "the message was not removed");
	pc_pass();
}
