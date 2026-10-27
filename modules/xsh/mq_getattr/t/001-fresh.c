/*
 * mq_getattr() on a fresh queue gives the creation attributes, no flags and no messages.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mq_getattr.001
 */
#include "mqh.h"

int main(void)
{
	struct mq_attr at;
	const char *n;
	mqd_t q;

	pc_begin("XSH.mq_getattr.001");
	n = mq_name("a");
	q = mq_new(n, O_RDWR, 4, 64);
	memset(&at, 0x55, sizeof at);
	pc_test();
	pc_check(mq_getattr(q, &at) == 0, "mq_getattr failed: %s", pc_errname(errno));
	pc_check(at.mq_maxmsg == 4, "mq_maxmsg is %ld, not 4", (long)at.mq_maxmsg);
	pc_check(at.mq_msgsize == 64, "mq_msgsize is %ld, not 64", (long)at.mq_msgsize);
	pc_check(at.mq_curmsgs == 0, "mq_curmsgs is %ld, not 0", (long)at.mq_curmsgs);
	pc_check((at.mq_flags & O_NONBLOCK) == 0, "mq_flags has O_NONBLOCK");
	pc_pass();
}
