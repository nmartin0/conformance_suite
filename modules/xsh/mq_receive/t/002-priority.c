/*
 * mq_receive() takes the oldest message of the highest priority.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mq_receive.002
 */
#include "mqh.h"

int main(void)
{
	static const unsigned prios[5] = { 2, 9, 9, 2, 9 };
	static const char want[] = "23514";
	static const unsigned wantp[5] = { 9, 9, 9, 2, 2 };
	const char *n;
	char buf[8], c;
	unsigned p;
	mqd_t q;
	int i;

	pc_begin("XSH.mq_receive.002");
	n = mq_name("a");
	q = mq_new(n, O_RDWR, 8, 8);
	for (i = 0; i < 5; i++) {
		c = (char)('1' + i);
		NEED(mq_send(q, &c, 1, prios[i]) == 0, "mq_send failed");
	}
	pc_test();
	for (i = 0; i < 5; i++) {
		pc_check(mq_receive(q, buf, sizeof buf, &p) == 1, "mq_receive %d failed", i);
		pc_check(buf[0] == want[i], "receive %d gave '%c', expected '%c'", i, buf[0], want[i]);
		pc_check(p == wantp[i], "receive %d gave priority %u, expected %u", i, p, wantp[i]);
	}
	pc_pass();
}
