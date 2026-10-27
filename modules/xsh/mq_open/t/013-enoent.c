/*
 * mq_open() without O_CREAT of a name that does not exist fails with ENOENT.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mq_open.013
 */
#include "mqh.h"

int main(void)
{
	const char *n;
	mqd_t q;

	pc_begin("XSH.mq_open.013");
	n = mq_name("a");
	pc_test();
	errno = 0;
	q = mq_open(n, O_RDWR);
	pc_check(q == BADQ, "mq_open of a missing queue succeeded");
	pc_check(errno == ENOENT, "errno is %s, not ENOENT", pc_errname(errno));
	pc_pass();
}
