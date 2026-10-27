/*
 * A descriptor that is a file descriptor is closed by mq_close().
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mq_close.004
 */
#include "mqh.h"

int main(void)
{
	const char *n;
	int before, during, after;
	mqd_t q;

	pc_begin("XSH.mq_close.004");
	n = mq_name("a");
	(void)mq_new(n, O_RDWR, 4, 64);
	{
		mqd_t t = mq_again(n, O_RDWR);

		NEED(mq_close(t) == 0, "mq_close failed");
	}
	before = pc_lowest_free_fd();
	q = mq_again(n, O_RDWR);
	during = pc_lowest_free_fd();
	if (during == before) {
		(void)mq_close(q);
		pc_notinuse("opening a queue takes no file descriptor here");
	}
	pc_test();
	pc_check(mq_close(q) == 0, "mq_close failed: %s", pc_errname(errno));
	after = pc_lowest_free_fd();
	pc_check(after == before, "the lowest free descriptor is %d after mq_close, %d before mq_open", after, before);
	pc_pass();
}
