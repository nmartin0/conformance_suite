/*
 * imaxabs: small.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.imaxabs.001
 */
#include "pc.h"
#include <inttypes.h>
static intmax_t (*volatile f_imaxabs)(intmax_t) = imaxabs;

int main(void)
{
	pc_begin("XSH.imaxabs.001");
	pc_test();
	pc_check(f_imaxabs(0) == 0, "imaxabs(0) was not 0");
	pc_check(f_imaxabs(1) == 1, "imaxabs(1) was not 1");
	pc_check(f_imaxabs(-1) == 1, "imaxabs(-1) was not 1");
	pc_check(f_imaxabs(12345) == 12345, "imaxabs(12345) was wrong");
	pc_check(f_imaxabs(-12345) == 12345, "imaxabs(-12345) was wrong");
	pc_check(f_imaxabs(-127) == 127, "imaxabs(-127) was wrong");
	pc_pass();
}
