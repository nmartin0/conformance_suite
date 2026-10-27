/*
 * TSVTX is 01000.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.tar.h.004
 */
#include <tar.h>
#include "pc.h"

static const struct { const char *name; unsigned long expect; unsigned long value; int missing; } v[] = {
#ifdef TSVTX
	{ "TSVTX", 01000, (unsigned long)(TSVTX), 0 },
#else
	{ "TSVTX", 01000, 0, 1 },
#endif
};

int main(void)
{
	size_t i;

	pc_begin("XBD.tar.h.004");
	pc_test();
	for (i = 0; i < sizeof v / sizeof v[0]; i++) {
		pc_check(!v[i].missing, "the header does not define %s", v[i].name);
		pc_check(v[i].value == v[i].expect, "%s is %#lo, not %#lo", v[i].name, v[i].value, v[i].expect);
	}
	pc_pass();
}
