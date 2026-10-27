/*
 * atoi: no conversion.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.atoi.003
 */
#include "pc.h"
#include <stdlib.h>
static int (*volatile f_atoi)(const char *) = atoi;

int main(void)
{
	static const char *const t[] = { "", "   ", "abc", "+", "-", "+ 1", "x12" };
	size_t k;
	pc_begin("XSH.atoi.003");
	pc_test();
	for (k = 0; k < sizeof t / sizeof t[0]; k++)
		pc_check(f_atoi(t[k]) == 0, "atoi(\"%s\") did not give 0", t[k]);
	pc_pass();
}
