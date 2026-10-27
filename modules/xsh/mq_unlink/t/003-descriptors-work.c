/*
 * Descriptors open when mq_unlink() is called keep working.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mq_unlink.003
 */
#include "mqh.h"

static int unlink_open(const char *n)
{
	int r;

	alarm_handler_on();
	alarm(3);
	r = mq_unlink(n);
	if (r == -1 && errno == EINTR)
		pc_unresolved("mq_unlink waited for the last close of the queue");
	alarm(0);
	return r;
}

int main(void)
{
	char buf[64];
	const char *n;
	mqd_t q1, q2;

	pc_begin("XSH.mq_unlink.003");
	n = mq_name("a");
	q1 = mq_new(n, O_RDWR, 4, 64);
	q2 = mq_again(n, O_RDWR);
	NEED(mq_send(q1, "one", 3, 0) == 0, "mq_send failed");
	pc_test();
	pc_check(unlink_open(n) == 0, "mq_unlink failed: %s", pc_errname(errno));
	pc_check(mq_send(q1, "two", 3, 0) == 0, "mq_send after mq_unlink failed: %s", pc_errname(errno));
	pc_check(mq_receive(q2, buf, sizeof buf, NULL) == 3 && memcmp(buf, "one", 3) == 0,
	    "the other descriptor does not see the first message");
	pc_check(mq_receive(q2, buf, sizeof buf, NULL) == 3 && memcmp(buf, "two", 3) == 0,
	    "the other descriptor does not see the second message");
	pc_pass();
}
