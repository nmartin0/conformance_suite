/*
 * mq_notify() with a NULL notification removes the registration.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mq_notify.005
 */
#include "mqh.h"

int main(void)
{
	struct sigevent ev;
	siginfo_t info;
	sigset_t set;
	const char *n;
	mqd_t q1, q2;

	pc_begin("XSH.mq_notify.005");
	n = mq_name("a");
	q1 = mq_new(n, O_RDWR, 4, 64);
	q2 = mq_again(n, O_RDWR);
	rt_block(&set);
	ev = ev_signal(1);
	NEED(mq_notify(q1, &ev) == 0, "mq_notify failed: %s", pc_errname(errno));
	pc_test();
	pc_check(mq_notify(q1, NULL) == 0, "removing the registration failed: %s", pc_errname(errno));
	pc_check(mq_send(q1, "m", 1, 0) == 0, "mq_send failed");
	pc_check(!rt_wait(&set, &info, 300), "a signal came after the registration was removed");
	pc_check(curmsgs(q1) == 1, "the message is not in the queue");
	{
		char buf[64];

		NEED(mq_receive(q2, buf, sizeof buf, NULL) == 1, "mq_receive failed");
	}
	pc_check(mq_notify(q2, &ev) == 0, "the queue cannot be registered after the removal: %s", pc_errname(errno));
	pc_pass();
}
