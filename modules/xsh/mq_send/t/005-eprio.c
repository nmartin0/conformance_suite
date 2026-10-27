/*
 * Priority MQ_PRIO_MAX-1 is accepted; MQ_PRIO_MAX fails with EINVAL and nothing is queued.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mq_send.005
 */
#include "mqh.h"

int main(void)
{
	const char *n;
	unsigned pm, p;
	char buf[8];
	mqd_t q;

	pc_begin("XSH.mq_send.005");
	n = mq_name("a");
	q = mq_new(n, O_RDWR, 4, 8);
	pm = prio_max();
	pc_test();
	pc_check(mq_send(q, "t", 1, pm - 1) == 0, "priority %u was refused: %s", pm - 1, pc_errname(errno));
	pc_check(mq_receive(q, buf, sizeof buf, &p) == 1 && p == pm - 1, "the priority did not come back");
	errno = 0;
	pc_check(mq_send(q, "t", 1, pm) == -1, "priority %u was accepted", pm);
	pc_check(errno == EINVAL, "errno is %s, not EINVAL", pc_errname(errno));
	pc_check(curmsgs(q) == 0, "a message was queued despite the error");
	pc_pass();
}
