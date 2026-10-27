/*
 * The mq_flags and mq_curmsgs members of attr are ignored when creating.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mq_open.006
 */
#include "mqh.h"

int main(void)
{
	struct mq_attr at, now;
	const char *n;
	mqd_t q;

	pc_begin("XSH.mq_open.006");
	n = mq_name("a");
	memset(&at, 0, sizeof at);
	at.mq_maxmsg = 4;
	at.mq_msgsize = 64;
	at.mq_flags = O_NONBLOCK;
	at.mq_curmsgs = 7;
	pc_test();
	q = mq_open(n, O_RDWR | O_CREAT | O_EXCL, (mode_t)0600, &at);
	pc_check(q != BADQ, "mq_open failed: %s", pc_errname(errno));
	pc_check(mq_getattr(q, &now) == 0, "mq_getattr failed");
	pc_check(now.mq_curmsgs == 0, "mq_curmsgs is %ld, not 0", (long)now.mq_curmsgs);
	pc_check((now.mq_flags & O_NONBLOCK) == 0, "O_NONBLOCK in attr.mq_flags took effect");
	pc_pass();
}
