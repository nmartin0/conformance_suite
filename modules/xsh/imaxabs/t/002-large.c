/*
 * imaxabs: large.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.imaxabs.002
 */
#include "pc.h"
#include <inttypes.h>
static intmax_t (*volatile f_imaxabs)(intmax_t) = imaxabs;

int main(void)
{
	pc_begin("XSH.imaxabs.002");
	pc_test();
	pc_check(f_imaxabs(-5000000000LL) == 5000000000LL, "imaxabs(-5000000000) was wrong");
	pc_check(f_imaxabs(5000000000LL) == 5000000000LL, "imaxabs(5000000000) was wrong");
	pc_check(f_imaxabs(-4294967296LL) == 4294967296LL, "imaxabs(-4294967296) was wrong");
	pc_pass();
}
