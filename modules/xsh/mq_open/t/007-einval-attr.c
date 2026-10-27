/*
 * mq_open() with O_CREAT and a non-positive mq_maxmsg or mq_msgsize fails with EINVAL.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mq_open.007
 */
#include "mqh.h"

static long cases[4][2] = { { 0, 64 }, { -1, 64 }, { 4, 0 }, { 4, -1 } };

int main(void)
{
	struct mq_attr at;
	const char *n;
	mqd_t q;
	int i, e;

	pc_begin("XSH.mq_open.007");
	n = mq_name("a");
	pc_test();
	for (i = 0; i < 4; i++) {
		memset(&at, 0, sizeof at);
		at.mq_maxmsg = cases[i][0];
		at.mq_msgsize = cases[i][1];
		errno = 0;
		q = mq_open(n, O_RDWR | O_CREAT | O_EXCL, (mode_t)0600, &at);
		e = errno;
		pc_check(q == BADQ, "mq_open with %ld/%ld succeeded", cases[i][0], cases[i][1]);
		pc_check(e == EINVAL, "mq_open with %ld/%ld: errno is %s, not EINVAL",
		    cases[i][0], cases[i][1], pc_errname(e));
	}
	pc_pass();
}
