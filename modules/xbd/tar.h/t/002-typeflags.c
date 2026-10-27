/*
 * The tar typeflag constants are the characters the page gives.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.tar.h.002
 */
#include <tar.h>
#include "pc.h"

static const struct { const char *name; int expect; int value; int missing; } v[] = {
#ifdef REGTYPE
	{ "REGTYPE", '0', (int)(REGTYPE), 0 },
#else
	{ "REGTYPE", '0', 0, 1 },
#endif
#ifdef AREGTYPE
	{ "AREGTYPE", '\0', (int)(AREGTYPE), 0 },
#else
	{ "AREGTYPE", '\0', 0, 1 },
#endif
#ifdef LNKTYPE
	{ "LNKTYPE", '1', (int)(LNKTYPE), 0 },
#else
	{ "LNKTYPE", '1', 0, 1 },
#endif
#ifdef SYMTYPE
	{ "SYMTYPE", '2', (int)(SYMTYPE), 0 },
#else
	{ "SYMTYPE", '2', 0, 1 },
#endif
#ifdef CHRTYPE
	{ "CHRTYPE", '3', (int)(CHRTYPE), 0 },
#else
	{ "CHRTYPE", '3', 0, 1 },
#endif
#ifdef BLKTYPE
	{ "BLKTYPE", '4', (int)(BLKTYPE), 0 },
#else
	{ "BLKTYPE", '4', 0, 1 },
#endif
#ifdef DIRTYPE
	{ "DIRTYPE", '5', (int)(DIRTYPE), 0 },
#else
	{ "DIRTYPE", '5', 0, 1 },
#endif
#ifdef FIFOTYPE
	{ "FIFOTYPE", '6', (int)(FIFOTYPE), 0 },
#else
	{ "FIFOTYPE", '6', 0, 1 },
#endif
#ifdef CONTTYPE
	{ "CONTTYPE", '7', (int)(CONTTYPE), 0 },
#else
	{ "CONTTYPE", '7', 0, 1 },
#endif
};

int main(void)
{
	size_t i;

	pc_begin("XBD.tar.h.002");
	pc_test();
	for (i = 0; i < sizeof v / sizeof v[0]; i++) {
		pc_check(!v[i].missing, "<tar.h> does not define %s", v[i].name);
		pc_check(v[i].value == v[i].expect, "%s is %d, not %d", v[i].name, v[i].value, v[i].expect);
	}
	pc_pass();
}
