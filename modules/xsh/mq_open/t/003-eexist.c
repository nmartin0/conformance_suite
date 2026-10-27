/*
 * mq_open() with O_CREAT|O_EXCL of an existing queue fails with EEXIST.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mq_open.003
 */
#include "mqh.h"

int main(void)
{
	struct mq_attr at;
	const char *n;
	mqd_t q, q2;

	pc_begin("XSH.mq_open.003");
	n = mq_name("a");
	q = mq_new(n, O_RDWR, 4, 64);
	memset(&at, 0, sizeof at);
	at.mq_maxmsg = 4;
	at.mq_msgsize = 64;
	pc_test();
	errno = 0;
	q2 = mq_open(n, O_RDWR | O_CREAT | O_EXCL, (mode_t)0600, &at);
	pc_check(q2 == BADQ, "O_CREAT|O_EXCL succeeded on an existing queue");
	pc_check(errno == EEXIST, "errno is %s, not EEXIST", pc_errname(errno));
	(void)q;
	pc_pass();
}
