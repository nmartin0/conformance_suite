/*
 * A zero-length message can be sent.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mq_send.003
 */
#include "mqh.h"

int main(void)
{
	char buf[64];
	const char *n;
	mqd_t q;

	pc_begin("XSH.mq_send.003");
	n = mq_name("a");
	q = mq_new(n, O_RDWR, 4, 64);
	pc_test();
	pc_check(mq_send(q, "", 0, 0) == 0, "mq_send of a zero-length message failed: %s", pc_errname(errno));
	pc_check(curmsgs(q) == 1, "the message was not queued");
	pc_check(mq_receive(q, buf, sizeof buf, NULL) == 0, "the received length is not 0");
	pc_pass();
}
