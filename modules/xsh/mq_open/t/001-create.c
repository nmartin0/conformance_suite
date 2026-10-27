/*
 * mq_open() with O_CREAT|O_EXCL makes a new, empty queue.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mq_open.001
 */
#include "mqh.h"

int main(void)
{
	struct mq_attr at;
	const char *n;
	mqd_t q;

	pc_begin("XSH.mq_open.001");
	n = mq_name("a");
	memset(&at, 0, sizeof at);
	at.mq_maxmsg = 4;
	at.mq_msgsize = 64;
	pc_test();
	q = mq_open(n, O_RDWR | O_CREAT | O_EXCL, (mode_t)0600, &at);
	pc_check(q != BADQ, "mq_open(O_CREAT|O_EXCL) failed: %s", pc_errname(errno));
	pc_check(curmsgs(q) == 0, "a new queue is not empty");
	pc_check(mq_close(q) == 0, "mq_close failed");
	pc_pass();
}
