/*
 * atoi: base ten only.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.atoi.004
 */
#include "pc.h"
#include <stdlib.h>
static int (*volatile f_atoi)(const char *) = atoi;

int main(void)
{
	pc_begin("XSH.atoi.004");
	pc_test();
	pc_check(f_atoi("0x1A") == 0, "atoi(\"0x1A\") did not give 0");
	pc_check(f_atoi("010") == 10, "atoi(\"010\") did not give 10");
	pc_check(f_atoi("1e3") == 1, "atoi(\"1e3\") did not give 1");
	pc_check(f_atoi("0b11") == 0, "atoi(\"0b11\") did not give 0");
	pc_pass();
}
