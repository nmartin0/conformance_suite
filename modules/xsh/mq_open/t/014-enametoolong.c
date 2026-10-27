/*
 * A very long name: mq_open() fails with ENAMETOOLONG (or EINVAL), or the name works with mq_unlink().
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mq_open.014
 */
#include "mqh.h"

#define LEN 4200

int main(void)
{
	static char big[LEN + 1];
	struct mq_attr at;
	char pre[32];
	mqd_t q;
	int e, r;

	pc_begin("XSH.mq_open.014");
	/* Unique per process, so a leftover or a parallel run cannot collide. */
	memset(big, 'a', LEN);
	snprintf(pre, sizeof pre, "/pcmq%ldx", (long)getpid());
	memcpy(big, pre, strlen(pre));
	memset(&at, 0, sizeof at);
	at.mq_maxmsg = 4;
	at.mq_msgsize = 64;
	pc_test();
	errno = 0;
	q = mq_open(big, O_RDWR | O_CREAT | O_EXCL, (mode_t)0600, &at);
	e = errno;
	if (q == BADQ) {
		pc_check(e == ENAMETOOLONG || e == EINVAL, "errno is %s, not ENAMETOOLONG", pc_errname(e));
		pc_pass();
	}
	/* Accepted: the same name must then not be refused as too long. */
	(void)mq_close(q);
	errno = 0;
	r = mq_unlink(big);
	e = errno;
	pc_check(r == 0 || e != ENAMETOOLONG, "mq_unlink of a name mq_open accepted gave ENAMETOOLONG");
	pc_check(r == 0, "mq_unlink of a name mq_open accepted failed: %s", pc_errname(e));
	pc_pass();
}
