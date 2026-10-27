/*
 * atof: prefix and no conversion.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.atof.002
 */
#include "pc.h"
#include <stdlib.h>
static double (*volatile f_atof)(const char *) = atof;

int main(void)
{
	static const struct { const char *s; double v; } t[] = {
		{ " \t\n\v\f\r4.5", 4.5 }, { "3.5xyz", 3.5 }, { "1e", 1.0 }, { "2.5.5", 2.5 }, { "", 0.0 }, { "   ", 0.0 }, { "abc", 0.0 }, { ".", 0.0 }, { "e5", 0.0 }
	};
	size_t k;
	pc_begin("XSH.atof.002");
	pc_test();
	for (k = 0; k < sizeof t / sizeof t[0]; k++)
		pc_check(f_atof(t[k].s) == t[k].v, "atof(\"%s\") did not give %g", t[k].s, t[k].v);
	pc_pass();
}
