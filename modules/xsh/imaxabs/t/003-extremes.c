/*
 * imaxabs: extremes.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.imaxabs.003
 */
#include "pc.h"
#include <inttypes.h>
static intmax_t (*volatile f_imaxabs)(intmax_t) = imaxabs;

int main(void)
{
	pc_begin("XSH.imaxabs.003");
	pc_test();
	pc_check(f_imaxabs(INTMAX_MAX) == INTMAX_MAX, "imaxabs(INTMAX_MAX) was wrong");
	pc_check(f_imaxabs(-INTMAX_MAX) == INTMAX_MAX, "imaxabs(-INTMAX_MAX) was wrong");
	pc_check(f_imaxabs(INTMAX_MIN + 1) == INTMAX_MAX, "imaxabs(INTMAX_MIN + 1) was wrong");
	pc_pass();
}
