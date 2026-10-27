/*
 * atof: values.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.atof.001
 */
#include "pc.h"
#include <stdlib.h>
static double (*volatile f_atof)(const char *) = atof;

int main(void)
{
	static const struct { const char *s; double v; } t[] = {
		{ "0", 0.0 }, { "1.5", 1.5 }, { "-2.25", -2.25 }, { "+2.5", 2.5 }, { ".5", 0.5 }, { "5.", 5.0 },
		{ "100", 100.0 }, { "0.25e1", 2.5 }, { "125e-2", 1.25 }, { "2E+3", 2000.0 }
	};
	size_t k;
	pc_begin("XSH.atof.001");
	pc_test();
	for (k = 0; k < sizeof t / sizeof t[0]; k++)
		pc_check(f_atof(t[k].s) == t[k].v, "atof(\"%s\") did not give %g", t[k].s, t[k].v);
	pc_pass();
}
