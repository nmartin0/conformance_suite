/*
 * mq_unlink() of a name that does not exist fails with ENOENT.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mq_unlink.004
 */
#include "mqh.h"

int main(void)
{
	const char *n;
	mqd_t q;

	pc_begin("XSH.mq_unlink.004");
	n = mq_name("a");
	pc_test();
	errno = 0;
	pc_check(mq_unlink(n) == -1, "mq_unlink of a missing queue succeeded");
	pc_check(errno == ENOENT, "errno is %s, not ENOENT", pc_errname(errno));
	q = mq_new(n, O_RDWR, 4, 64);
	NEED(mq_close(q) == 0, "mq_close failed");
	pc_check(mq_unlink(n) == 0, "mq_unlink of an existing queue failed: %s", pc_errname(errno));
	errno = 0;
	pc_check(mq_unlink(n) == -1, "the second mq_unlink succeeded");
	pc_check(errno == ENOENT, "errno of the second mq_unlink is %s, not ENOENT", pc_errname(errno));
	pc_pass();
}
