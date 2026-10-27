/*
 * A registration ends with its notification: the next arrival sends nothing, and the queue can be registered again.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mq_notify.002
 */
#include "mqh.h"

int main(void)
{
	struct sigevent ev;
	siginfo_t info;
	sigset_t set;
	char buf[64];
	const char *n;
	mqd_t q;

	pc_begin("XSH.mq_notify.002");
	n = mq_name("a");
	q = mq_new(n, O_RDWR, 4, 64);
	rt_block(&set);
	ev = ev_signal(5);
	NEED(mq_notify(q, &ev) == 0, "mq_notify failed: %s", pc_errname(errno));
	NEED(mq_send(q, "1", 1, 0) == 0, "mq_send failed");
	NEED(rt_wait(&set, &info, 5000), "the first notification did not come");
	NEED(mq_receive(q, buf, sizeof buf, NULL) == 1, "mq_receive failed");
	pc_test();
	pc_check(mq_send(q, "2", 1, 0) == 0, "mq_send failed");
	pc_check(!rt_wait(&set, &info, 300), "a second signal came without a new registration");
	pc_check(mq_receive(q, buf, sizeof buf, NULL) == 1, "mq_receive failed");
	pc_check(mq_notify(q, &ev) == 0, "registering again failed: %s", pc_errname(errno));
	pc_check(mq_send(q, "3", 1, 0) == 0, "mq_send failed");
	pc_check(rt_wait(&set, &info, 5000), "no signal for the new registration");
	pc_pass();
}
