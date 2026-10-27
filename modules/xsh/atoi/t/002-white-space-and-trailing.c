/*
 * atoi: white space and trailing.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.atoi.002
 */
#include "pc.h"
#include <stdlib.h>
static int (*volatile f_atoi)(const char *) = atoi;

int main(void)
{
	static const struct { const char *s; int v; } t[] = { { " \t\n\v\f\r12", 12 }, { " 7", 7 }, { "\t-3", -3 }, { "99 bottles", 99 }, { "5-3", 5 }, { "12abc", 12 }, { "-8x", -8 }, { "3.9", 3 } };
	size_t k;
	pc_begin("XSH.atoi.002");
	pc_test();
	for (k = 0; k < sizeof t / sizeof t[0]; k++)
		pc_check(f_atoi(t[k].s) == t[k].v, "atoi(\"%s\") did not give %d", t[k].s, t[k].v);
	pc_pass();
}
