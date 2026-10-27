/*
 * A zero-length message is received with length 0 and removed.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mq_receive.008
 */
#include "mqh.h"

int main(void)
{
	char buf[64];
	const char *n;
	unsigned p = 99;
	mqd_t q;

	pc_begin("XSH.mq_receive.008");
	n = mq_name("a");
	q = mq_new(n, O_RDWR, 4, 64);
	NEED(mq_send(q, "", 0, 6) == 0, "mq_send of a zero-length message failed");
	pc_test();
	pc_check(mq_receive(q, buf, sizeof buf, &p) == 0, "the length is not 0");
	pc_check(p == 6, "the priority is %u, not 6", p);
	pc_check(curmsgs(q) == 0, "the message was not removed");
	pc_pass();
}
