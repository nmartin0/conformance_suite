/*
 * mq_timedsend() of more than mq_msgsize bytes fails with EMSGSIZE and queues nothing.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mq_timedsend.008
 */
#include "mqh.h"

int main(void)
{
	struct timespec to;
	char big[9] = "123456789";
	const char *n;
	mqd_t q;

	pc_begin("XSH.mq_timedsend.008");
	n = mq_name("a");
	q = mq_new(n, O_RDWR, 4, 8);
	pc_test();
	to = abs_in(5000);
	pc_check(mq_timedsend(q, big, 8, 0, &to) == 0, "mq_timedsend of 8 bytes failed: %s", pc_errname(errno));
	errno = 0;
	pc_check(mq_timedsend(q, big, 9, 0, &to) == -1, "mq_timedsend of 9 bytes succeeded");
	pc_check(errno == EMSGSIZE, "errno is %s, not EMSGSIZE", pc_errname(errno));
	pc_check(curmsgs(q) == 1, "mq_curmsgs is %ld, not 1", curmsgs(q));
	pc_pass();
}
