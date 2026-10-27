/*
 * mq_send() queues exactly msg_len bytes; later changes to the buffer do not alter the message.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mq_send.002
 */
#include "mqh.h"

int main(void)
{
	char buf[64], src[8] = "hello";
	const char *n;
	unsigned p = 99;
	mqd_t q;

	pc_begin("XSH.mq_send.002");
	n = mq_name("a");
	q = mq_new(n, O_RDWR, 4, 64);
	pc_test();
	pc_check(mq_send(q, src, 5, 7) == 0, "mq_send failed: %s", pc_errname(errno));
	memcpy(src, "XXXXX", 5);
	pc_check(curmsgs(q) == 1, "mq_curmsgs is not 1");
	pc_check(mq_receive(q, buf, sizeof buf, &p) == 5, "the received length is not 5");
	pc_check(memcmp(buf, "hello", 5) == 0, "the message content changed");
	pc_check(p == 7, "the priority is %u, not 7", p);
	pc_pass();
}
