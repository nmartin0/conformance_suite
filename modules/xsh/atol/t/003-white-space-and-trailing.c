/*
 * atol: white space and trailing.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.atol.003
 */
#include "pc.h"
#include <stdlib.h>
static long (*volatile f_atol)(const char *) = atol;
static long long (*volatile f_atoll)(const char *) = atoll;

int main(void)
{
	static const struct { const char *s; long v; } t[] = { { " \t\n\v\f\r12", 12 }, { "99 bottles", 99 }, { "-8x", -8 }, { "0x1A", 0 }, { "", 0 }, { "abc", 0 }, { "+", 0 }, { "010", 10 }, { "   ", 0 } };
	size_t k;
	pc_begin("XSH.atol.003");
	pc_test();
	for (k = 0; k < sizeof t / sizeof t[0]; k++) {
		pc_check(f_atol(t[k].s) == t[k].v, "atol(\"%s\") did not give %ld", t[k].s, t[k].v);
		pc_check(f_atoll(t[k].s) == t[k].v, "atoll(\"%s\") did not give %ld", t[k].s, t[k].v);
	}
	pc_pass();
}
