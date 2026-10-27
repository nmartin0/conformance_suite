/*
 * atoi: values.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.atoi.001
 */
#include "pc.h"
#include <stdlib.h>
static int (*volatile f_atoi)(const char *) = atoi;

int main(void)
{
	static const struct { const char *s; int v; } t[] = { { "0", 0 }, { "7", 7 }, { "42", 42 }, { "007", 7 }, { "-17", -17 }, { "+5", 5 }, { "-0", 0 }, { "2147", 2147 } };
	size_t k;
	pc_begin("XSH.atoi.001");
	pc_test();
	for (k = 0; k < sizeof t / sizeof t[0]; k++)
		pc_check(f_atoi(t[k].s) == t[k].v, "atoi(\"%s\") did not give %d", t[k].s, t[k].v);
	pc_pass();
}
