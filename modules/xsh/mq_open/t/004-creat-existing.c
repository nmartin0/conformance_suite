/*
 * O_CREAT on an existing queue changes nothing: attributes and messages stay.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mq_open.004
 */
#include "mqh.h"

int main(void)
{
	struct mq_attr at, now;
	char buf[64];
	const char *n;
	mqd_t q, q2;

	pc_begin("XSH.mq_open.004");
	n = mq_name("a");
	q = mq_new(n, O_RDWR, 4, 64);
	NEED(mq_send(q, "keep", 4, 0) == 0, "mq_send failed");
	memset(&at, 0, sizeof at);
	at.mq_maxmsg = 9;
	at.mq_msgsize = 9;
	pc_test();
	q2 = mq_open(n, O_RDWR | O_CREAT, (mode_t)0600, &at);
	pc_check(q2 != BADQ, "mq_open(O_CREAT) of an existing queue failed: %s", pc_errname(errno));
	pc_check(mq_getattr(q2, &now) == 0, "mq_getattr failed");
	pc_check(now.mq_maxmsg == 4 && now.mq_msgsize == 64, "attributes changed to %ld/%ld",
	    (long)now.mq_maxmsg, (long)now.mq_msgsize);
	pc_check(now.mq_curmsgs == 1, "mq_curmsgs is %ld, not 1", (long)now.mq_curmsgs);
	pc_check(mq_receive(q2, buf, sizeof buf, NULL) == 4 && memcmp(buf, "keep", 4) == 0,
	    "the message was lost");
	pc_pass();
}
