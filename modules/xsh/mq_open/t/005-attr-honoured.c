/*
 * mq_open() creates the queue with the requested mq_maxmsg and mq_msgsize.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mq_open.005
 */
#include "mqh.h"

static void check(const char *n, long maxmsg, long msgsize)
{
	struct mq_attr at, now;
	mqd_t q;

	memset(&at, 0, sizeof at);
	at.mq_maxmsg = maxmsg;
	at.mq_msgsize = msgsize;
	q = mq_open(n, O_RDWR | O_CREAT | O_EXCL, (mode_t)0600, &at);
	pc_check(q != BADQ, "mq_open with %ld/%ld failed: %s", maxmsg, msgsize, pc_errname(errno));
	pc_check(mq_getattr(q, &now) == 0, "mq_getattr failed");
	pc_check(now.mq_maxmsg == maxmsg, "mq_maxmsg is %ld, not %ld", (long)now.mq_maxmsg, maxmsg);
	pc_check(now.mq_msgsize == msgsize, "mq_msgsize is %ld, not %ld", (long)now.mq_msgsize, msgsize);
}

int main(void)
{
	const char *a, *b;

	pc_begin("XSH.mq_open.005");
	a = mq_name("a");
	b = mq_name("b");
	pc_test();
	check(a, 4, 64);
	check(b, 3, 10);
	pc_pass();
}
