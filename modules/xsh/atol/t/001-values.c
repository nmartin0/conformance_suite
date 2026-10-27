/*
 * atol: values.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.atol.001
 */
#include "pc.h"
#include <stdlib.h>
static long (*volatile f_atol)(const char *) = atol;
static long long (*volatile f_atoll)(const char *) = atoll;

int main(void)
{
	static const struct { const char *s; long v; } t[] = { { "0", 0 }, { "42", 42 }, { "-17", -17 }, { "+5", 5 }, { "007", 7 }, { "123456", 123456 }, { "-0", 0 } };
	size_t k;
	pc_begin("XSH.atol.001");
	pc_test();
	for (k = 0; k < sizeof t / sizeof t[0]; k++) {
		pc_check(f_atol(t[k].s) == t[k].v, "atol(\"%s\") did not give %ld", t[k].s, t[k].v);
		pc_check(f_atoll(t[k].s) == t[k].v, "atoll(\"%s\") did not give %ld", t[k].s, t[k].v);
	}
	pc_pass();
}
