/*
 * While a process is registered, any further mq_notify() registration in that process fails with EBUSY.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mq_notify.003
 */
#include "mqh.h"

int main(void)
{
	struct sigevent ev;
	sigset_t set;
	const char *n;
	mqd_t q1, q2;

	pc_begin("XSH.mq_notify.003");
	n = mq_name("a");
	q1 = mq_new(n, O_RDWR, 4, 64);
	q2 = mq_again(n, O_RDWR);
	rt_block(&set);
	ev = ev_signal(1);
	pc_test();
	pc_check(mq_notify(q1, &ev) == 0, "mq_notify failed: %s", pc_errname(errno));
	errno = 0;
	pc_check(mq_notify(q1, &ev) == -1, "registering twice through one descriptor succeeded");
	pc_check(errno == EBUSY, "errno is %s, not EBUSY", pc_errname(errno));
	errno = 0;
	pc_check(mq_notify(q2, &ev) == -1, "registering through another descriptor succeeded");
	pc_check(errno == EBUSY, "errno is %s, not EBUSY", pc_errname(errno));
	pc_pass();
}
