/*
 * mq_send() on a full non-blocking queue fails with EAGAIN and queues nothing.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mq_send.006
 */
#include "mqh.h"

int main(void)
{
	char buf[8];
	const char *n;
	mqd_t q;

	pc_begin("XSH.mq_send.006");
	n = mq_name("a");
	q = mq_new(n, O_RDWR | O_NONBLOCK, 3, 8);
	NEED(mq_send(q, "1", 1, 0) == 0 && mq_send(q, "2", 1, 0) == 0 && mq_send(q, "3", 1, 0) == 0,
	    "filling the queue failed");
	pc_test();
	errno = 0;
	pc_check(mq_send(q, "4", 1, 0) == -1, "mq_send to a full queue succeeded");
	pc_check(errno == EAGAIN, "errno is %s, not EAGAIN", pc_errname(errno));
	pc_check(curmsgs(q) == 3, "mq_curmsgs is %ld, not 3", curmsgs(q));
	pc_check(mq_receive(q, buf, sizeof buf, NULL) == 1 && buf[0] == '1', "the oldest message is not intact");
	pc_pass();
}
