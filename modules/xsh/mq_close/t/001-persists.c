/*
 * mq_close() returns 0; the queue and its messages persist for other descriptors and later opens.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mq_close.001
 */
#include "mqh.h"

int main(void)
{
	char buf[64];
	const char *n;
	mqd_t q1, q2, q3;

	pc_begin("XSH.mq_close.001");
	n = mq_name("a");
	q1 = mq_new(n, O_RDWR, 4, 64);
	q2 = mq_again(n, O_RDWR);
	NEED(mq_send(q1, "stay", 4, 0) == 0, "mq_send failed");
	pc_test();
	pc_check(mq_close(q1) == 0, "mq_close failed: %s", pc_errname(errno));
	pc_check(curmsgs(q2) == 1, "the message went away with the closed descriptor");
	pc_check(mq_close(q2) == 0, "mq_close of the last descriptor failed: %s", pc_errname(errno));
	q3 = mq_open(n, O_RDWR);
	pc_check(q3 != BADQ, "the queue is gone after every descriptor was closed: %s", pc_errname(errno));
	pc_check(mq_receive(q3, buf, sizeof buf, NULL) == 4 && memcmp(buf, "stay", 4) == 0,
	    "the message is gone after every descriptor was closed");
	pc_pass();
}
