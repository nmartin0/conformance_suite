/*
 * sem_unlink() of a name that never existed fails with ENOENT.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sem_unlink.002
 */
#include "semh.h"

int main(void)
{
	const char *n;
	int r;

	pc_begin("XSH.sem_unlink.002");
	guard(30);
	n = sn_make("a");
	pc_test();
	errno = 0;
	r = sem_unlink(n);
	pc_check(r == -1, "sem_unlink() of a name that does not exist returned %d, expected -1", r);
	pc_check(errno == ENOENT, "errno is %s, expected ENOENT", pc_errname(errno));
	pc_pass();
}
