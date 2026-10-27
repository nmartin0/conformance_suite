/*
 * With attr NULL the queue is created with usable default attributes.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mq_open.008
 */
#include "mqh.h"

int main(void)
{
	struct mq_attr now;
	char *buf;
	const char *n;
	mqd_t q;

	pc_begin("XSH.mq_open.008");
	n = mq_name("a");
	pc_test();
	q = mq_open(n, O_RDWR | O_CREAT | O_EXCL, (mode_t)0600, NULL);
	pc_check(q != BADQ, "mq_open with attr NULL failed: %s", pc_errname(errno));
	pc_check(mq_getattr(q, &now) == 0, "mq_getattr failed");
	pc_check(now.mq_maxmsg > 0 && now.mq_msgsize > 0, "default attributes %ld/%ld are not positive",
	    (long)now.mq_maxmsg, (long)now.mq_msgsize);
	pc_check(now.mq_curmsgs == 0, "the queue is not empty");
	buf = malloc((size_t)now.mq_msgsize);
	NEED(buf != NULL, "malloc failed");
	pc_check(mq_send(q, "h", 1, 0) == 0, "mq_send on a default queue failed: %s", pc_errname(errno));
	pc_check(mq_receive(q, buf, (size_t)now.mq_msgsize, NULL) == 1 && buf[0] == 'h',
	    "mq_receive on a default queue failed");
	free(buf);
	pc_pass();
}
