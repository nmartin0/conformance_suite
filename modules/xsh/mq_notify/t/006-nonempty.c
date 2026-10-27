/*
 * No notification is sent for a message arriving at a non-empty queue; the later empty-to-non-empty transition does notify.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mq_notify.006
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

	pc_begin("XSH.mq_notify.006");
	n = mq_name("a");
	q = mq_new(n, O_RDWR, 4, 64);
	rt_block(&set);
	ev = ev_signal(9);
	NEED(mq_send(q, "1", 1, 0) == 0, "mq_send failed");
	NEED(mq_notify(q, &ev) == 0, "registering on a queue holding a message failed: %s", pc_errname(errno));
	pc_test();
	pc_check(mq_send(q, "2", 1, 0) == 0, "mq_send failed");
	pc_check(!rt_wait(&set, &info, 300), "a signal came for a message arriving at a non-empty queue");
	pc_check(mq_receive(q, buf, sizeof buf, NULL) == 1 && mq_receive(q, buf, sizeof buf, NULL) == 1,
	    "mq_receive failed");
	pc_check(mq_send(q, "3", 1, 0) == 0, "mq_send failed");
	pc_check(rt_wait(&set, &info, 5000), "no signal when the empty queue received a message");
	pc_check(info.si_value.sival_int == 9, "si_value is %d, not 9", info.si_value.sival_int);
	pc_pass();
}
