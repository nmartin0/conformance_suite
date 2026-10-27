/*
 * mq_timedsend() with priority MQ_PRIO_MAX fails with EINVAL and queues nothing.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mq_timedsend.009
 */
#include "mqh.h"

int main(void)
{
	struct timespec to;
	const char *n;
	unsigned pm;
	mqd_t q;

	pc_begin("XSH.mq_timedsend.009");
	n = mq_name("a");
	q = mq_new(n, O_RDWR, 4, 8);
	pm = prio_max();
	pc_test();
	to = abs_in(5000);
	pc_check(mq_timedsend(q, "t", 1, pm - 1, &to) == 0, "priority %u was refused: %s", pm - 1, pc_errname(errno));
	errno = 0;
	pc_check(mq_timedsend(q, "t", 1, pm, &to) == -1, "priority %u was accepted", pm);
	pc_check(errno == EINVAL, "errno is %s, not EINVAL", pc_errname(errno));
	pc_check(curmsgs(q) == 1, "mq_curmsgs is %ld, not 1", curmsgs(q));
	pc_pass();
}
