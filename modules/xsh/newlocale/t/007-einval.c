/*
 * A mask bit that is no category gives EINVAL.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.newlocale.007
 */
#include "lc.h"

int main(void)
{
	locale_t r;
	int bit, k, e;

	pc_begin("XSH.newlocale.007");
	lc_init();
	bit = 0;
	for (k = 0; k < 30 && bit == 0; k++)
		if (((int)LC_ALL_MASK & (1 << k)) == 0)
			bit = 1 << k;
	if (bit == 0)
		pc_notinuse("LC_ALL_MASK covers every bit that could be tried");
	pc_test();
	errno = 0;
	r = newlocale(bit | LC_CTYPE_MASK, "C", (locale_t)0);
	e = errno;
	if (r != (locale_t)0)
		freelocale(r);
	pc_check(r == (locale_t)0, "newlocale() accepted a mask with a bit that is no category");
	pc_check(e == EINVAL, "newlocale() with a bad mask set errno to %s, expected EINVAL", pc_errname(e));
	pc_pass();
}
