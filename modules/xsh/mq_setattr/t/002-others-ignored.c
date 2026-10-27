/*
 * mq_setattr() ignores mq_maxmsg, mq_msgsize and mq_curmsgs of its input.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mq_setattr.002
 */
#include "mqh.h"

int main(void)
{
	struct mq_attr in, now;
	const char *n;
	mqd_t q;

	pc_begin("XSH.mq_setattr.002");
	n = mq_name("a");
	q = mq_new(n, O_RDWR, 4, 64);
	fill(q, 1);
	memset(&in, 0, sizeof in);
	in.mq_flags = O_NONBLOCK;
	in.mq_maxmsg = 99;
	in.mq_msgsize = 999;
	in.mq_curmsgs = 55;
	pc_test();
	pc_check(mq_setattr(q, &in, NULL) == 0, "mq_setattr failed: %s", pc_errname(errno));
	pc_check(mq_getattr(q, &now) == 0, "mq_getattr failed");
	pc_check(now.mq_maxmsg == 4, "mq_maxmsg is %ld, not 4", (long)now.mq_maxmsg);
	pc_check(now.mq_msgsize == 64, "mq_msgsize is %ld, not 64", (long)now.mq_msgsize);
	pc_check(now.mq_curmsgs == 1, "mq_curmsgs is %ld, not 1", (long)now.mq_curmsgs);
	pc_pass();
}
