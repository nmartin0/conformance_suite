/*
 * mq_timedsend() inserts by priority like mq_send().
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mq_timedsend.007
 */
#include "mqh.h"

int main(void)
{
	static const char order[] = "bdcaef";
	static const unsigned send_prio[] = { 1, 5, 3, 5, 1, 0 };
	struct timespec to;
	const char *n;
	char buf[8];
	unsigned p;
	mqd_t q;
	int i;

	pc_begin("XSH.mq_timedsend.007");
	n = mq_name("a");
	q = mq_new(n, O_RDWR, 8, 8);
	pc_test();
	for (i = 0; i < 6; i++) {
		char c = "abcdef"[i];

		to = abs_in(5000);
		pc_check(mq_timedsend(q, &c, 1, send_prio[i], &to) == 0, "mq_timedsend %d failed: %s", i, pc_errname(errno));
	}
	for (i = 0; i < 6; i++) {
		pc_check(mq_receive(q, buf, sizeof buf, &p) == 1, "mq_receive %d failed", i);
		pc_check(buf[0] == order[i], "message %d is '%c', expected '%c'", i, buf[0], order[i]);
	}
	pc_pass();
}
