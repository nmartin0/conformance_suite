/*
 * mq_timedsend() with room in the queue succeeds even if abstime has long passed.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mq_timedsend.001
 */
#include "mqh.h"

int main(void)
{
	struct timespec past = { 1, 0 };
	char buf[8];
	const char *n;
	mqd_t q;

	pc_begin("XSH.mq_timedsend.001");
	n = mq_name("a");
	q = mq_new(n, O_RDWR, 4, 8);
	pc_test();
	pc_check(mq_timedsend(q, "ok", 2, 3, &past) == 0, "mq_timedsend with room and a past time failed: %s", pc_errname(errno));
	pc_check(curmsgs(q) == 1, "the message was not queued");
	pc_check(mq_receive(q, buf, sizeof buf, NULL) == 2 && memcmp(buf, "ok", 2) == 0, "the message differs");
	pc_pass();
}
