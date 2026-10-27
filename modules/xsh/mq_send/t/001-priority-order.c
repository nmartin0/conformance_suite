/*
 * mq_send() inserts by priority: larger values first, FIFO among equal priorities.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mq_send.001
 */
#include "mqh.h"

int main(void)
{
	static const char order[] = "bdcaef";
	static const unsigned prios[] = { 5, 5, 3, 1, 1, 0 };
	static const unsigned send_prio[] = { 1, 5, 3, 5, 1, 0 };
	const char *n;
	char buf[8];
	unsigned p;
	mqd_t q;
	int i;

	pc_begin("XSH.mq_send.001");
	n = mq_name("a");
	q = mq_new(n, O_RDWR, 8, 8);
	pc_test();
	for (i = 0; i < 6; i++) {
		char c = "abcdef"[i];

		pc_check(mq_send(q, &c, 1, send_prio[i]) == 0, "mq_send %d failed: %s", i, pc_errname(errno));
	}
	for (i = 0; i < 6; i++) {
		pc_check(mq_receive(q, buf, sizeof buf, &p) == 1, "mq_receive %d failed", i);
		pc_check(buf[0] == order[i], "message %d is '%c', expected '%c'", i, buf[0], order[i]);
		pc_check(p == prios[i], "message %d has priority %u, expected %u", i, p, prios[i]);
	}
	pc_pass();
}
