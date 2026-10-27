/*
 * mq_receive() returns the message length, copies the message and stores its priority.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mq_receive.001
 */
#include "mqh.h"

int main(void)
{
	char buf[64];
	const char *n;
	unsigned p = 99;
	mqd_t q;

	pc_begin("XSH.mq_receive.001");
	n = mq_name("a");
	q = mq_new(n, O_RDWR, 4, 64);
	NEED(mq_send(q, "first", 5, 4) == 0 && mq_send(q, "second", 6, 4) == 0, "mq_send failed");
	pc_test();
	pc_check(mq_receive(q, buf, sizeof buf, &p) == 5, "the length is not 5");
	pc_check(memcmp(buf, "first", 5) == 0, "the content differs");
	pc_check(p == 4, "the priority is %u, not 4", p);
	pc_check(mq_receive(q, buf, sizeof buf, NULL) == 6, "with msg_prio NULL the length is not 6");
	pc_check(memcmp(buf, "second", 6) == 0, "the second content differs");
	pc_check(curmsgs(q) == 0, "received messages are still queued");
	pc_pass();
}
