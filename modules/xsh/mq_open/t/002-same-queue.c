/*
 * A leading-slash name refers to the same queue for every open, in any process.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mq_open.002
 */
#include "mqh.h"

int main(void)
{
	char buf[64];
	const char *n;
	mqd_t q1, q2;
	pid_t kid;

	pc_begin("XSH.mq_open.002");
	n = mq_name("a");
	q1 = mq_new(n, O_RDWR, 4, 64);
	pc_test();
	q2 = mq_open(n, O_RDWR);
	pc_check(q2 != BADQ, "mq_open of the existing name failed: %s", pc_errname(errno));
	pc_check(mq_send(q1, "ab", 2, 0) == 0, "mq_send failed");
	pc_check(mq_receive(q2, buf, sizeof buf, NULL) == 2, "a message sent through one descriptor is not received through the other");
	pc_check(memcmp(buf, "ab", 2) == 0, "wrong message content");
	kid = fork();
	NEED(kid != -1, "fork failed");
	if (kid == 0) {
		mqd_t c = mq_open(n, O_WRONLY);

		if (c == BADQ)
			_exit(10);
		_exit(mq_send(c, "kid", 3, 0) == 0 ? 0 : 11);
	}
	pc_check(reap(kid) == 0, "the child could not open the name or send");
	pc_check(mq_receive(q1, buf, sizeof buf, NULL) == 3 && memcmp(buf, "kid", 3) == 0,
	    "the message sent by the other process is not in the queue");
	pc_pass();
}
