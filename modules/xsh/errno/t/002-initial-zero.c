/*
 * errno is zero at program start.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.errno.002
 */
#include "pc.h"
#include <errno.h>

int main(void)
{
	int at_start = errno;	/* before any other call */

	pc_begin("XSH.errno.002");
	pc_test();
	pc_check(at_start == 0, "errno was %d at the start of main()", at_start);
	pc_pass();
}
