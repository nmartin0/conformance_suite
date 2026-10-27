/*
 * mq_setattr() of an invalid descriptor fails with EBADF.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mq_setattr.006
 */
#include "mqh.h"

int main(void)
{
	struct mq_attr in, old;

	pc_begin("XSH.mq_setattr.006");
	memset(&in, 0, sizeof in);
	memset(&old, 0, sizeof old);
	pc_test();
	errno = 0;
	pc_check(mq_setattr(BADQ, &in, &old) == -1, "mq_setattr of an invalid descriptor succeeded");
	pc_check(errno == EBADF, "errno is %s, not EBADF", pc_errname(errno));
	pc_pass();
}
