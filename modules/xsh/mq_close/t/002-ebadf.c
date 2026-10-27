/*
 * mq_close() of an invalid descriptor fails with EBADF.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mq_close.002
 */
#include "mqh.h"

int main(void)
{
	pc_begin("XSH.mq_close.002");
	pc_test();
	errno = 0;
	pc_check(mq_close(BADQ) == -1, "mq_close of an invalid descriptor succeeded");
	pc_check(errno == EBADF, "errno is %s, not EBADF", pc_errname(errno));
	pc_pass();
}
