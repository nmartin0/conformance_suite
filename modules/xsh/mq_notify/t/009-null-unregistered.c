/*
 * mq_notify() with a NULL notification when not registered returns 0 or fails with EINVAL.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mq_notify.009
 */
#include "mqh.h"

int main(void)
{
	const char *n;
	mqd_t q;
	int r;

	pc_begin("XSH.mq_notify.009");
	n = mq_name("a");
	q = mq_new(n, O_RDWR, 4, 64);
	pc_test();
	errno = 0;
	r = mq_notify(q, NULL);
	pc_check(r == 0 || (r == -1 && errno == EINVAL), "mq_notify(NULL) when not registered: result %d, errno %s",
	    r, pc_errname(errno));
	pc_pass();
}
