/*
 * atoi: int limits.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.atoi.005
 */
#include "pc.h"
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
static int (*volatile f_atoi)(const char *) = atoi;

int main(void)
{
	char b[32];
	pc_begin("XSH.atoi.005");
	pc_test();
	pc_check(snprintf(b, sizeof b, "%d", INT_MAX) > 0, "snprintf failed");
	pc_check(f_atoi(b) == INT_MAX, "atoi(\"%s\") did not give INT_MAX", b);
	pc_check(snprintf(b, sizeof b, "%d", INT_MIN) > 0, "snprintf failed");
	pc_check(f_atoi(b) == INT_MIN, "atoi(\"%s\") did not give INT_MIN", b);
	pc_pass();
}
