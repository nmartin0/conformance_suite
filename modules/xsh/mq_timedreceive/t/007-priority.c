/*
 * mq_timedreceive() takes the oldest message of the highest priority.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mq_timedreceive.007
 */
#include "mqh.h"

int main(void)
{
	static const unsigned prios[5] = { 2, 9, 9, 2, 9 };
	static const char want[] = "23514";
	struct timespec to;
	const char *n;
	char buf[8], c;
	unsigned p;
	mqd_t q;
	int i;

	pc_begin("XSH.mq_timedreceive.007");
	n = mq_name("a");
	q = mq_new(n, O_RDWR, 8, 8);
	for (i = 0; i < 5; i++) {
		c = (char)('1' + i);
		NEED(mq_send(q, &c, 1, prios[i]) == 0, "mq_send failed");
	}
	pc_test();
	for (i = 0; i < 5; i++) {
		to = abs_in(5000);
		pc_check(mq_timedreceive(q, buf, sizeof buf, &p, &to) == 1, "mq_timedreceive %d failed", i);
		pc_check(buf[0] == want[i], "receive %d gave '%c', expected '%c'", i, buf[0], want[i]);
		pc_check(p == prios[want[i] - '1'], "receive %d gave priority %u", i, p);
	}
	pc_pass();
}
