/*
 * The tar mode bit constants have the octal values the page gives.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.tar.h.003
 */
#include <tar.h>
#include "pc.h"

static const struct { const char *name; unsigned long expect; unsigned long value; int missing; } v[] = {
#ifdef TSUID
	{ "TSUID", 04000, (unsigned long)(TSUID), 0 },
#else
	{ "TSUID", 04000, 0, 1 },
#endif
#ifdef TSGID
	{ "TSGID", 02000, (unsigned long)(TSGID), 0 },
#else
	{ "TSGID", 02000, 0, 1 },
#endif
#ifdef TUREAD
	{ "TUREAD", 00400, (unsigned long)(TUREAD), 0 },
#else
	{ "TUREAD", 00400, 0, 1 },
#endif
#ifdef TUWRITE
	{ "TUWRITE", 00200, (unsigned long)(TUWRITE), 0 },
#else
	{ "TUWRITE", 00200, 0, 1 },
#endif
#ifdef TUEXEC
	{ "TUEXEC", 00100, (unsigned long)(TUEXEC), 0 },
#else
	{ "TUEXEC", 00100, 0, 1 },
#endif
#ifdef TGREAD
	{ "TGREAD", 00040, (unsigned long)(TGREAD), 0 },
#else
	{ "TGREAD", 00040, 0, 1 },
#endif
#ifdef TGWRITE
	{ "TGWRITE", 00020, (unsigned long)(TGWRITE), 0 },
#else
	{ "TGWRITE", 00020, 0, 1 },
#endif
#ifdef TGEXEC
	{ "TGEXEC", 00010, (unsigned long)(TGEXEC), 0 },
#else
	{ "TGEXEC", 00010, 0, 1 },
#endif
#ifdef TOREAD
	{ "TOREAD", 00004, (unsigned long)(TOREAD), 0 },
#else
	{ "TOREAD", 00004, 0, 1 },
#endif
#ifdef TOWRITE
	{ "TOWRITE", 00002, (unsigned long)(TOWRITE), 0 },
#else
	{ "TOWRITE", 00002, 0, 1 },
#endif
#ifdef TOEXEC
	{ "TOEXEC", 00001, (unsigned long)(TOEXEC), 0 },
#else
	{ "TOEXEC", 00001, 0, 1 },
#endif
};

int main(void)
{
	size_t i;

	pc_begin("XBD.tar.h.003");
	pc_test();
	for (i = 0; i < sizeof v / sizeof v[0]; i++) {
		pc_check(!v[i].missing, "the header does not define %s", v[i].name);
		pc_check(v[i].value == v[i].expect, "%s is %#lo, not %#lo", v[i].name, v[i].value, v[i].expect);
	}
	pc_pass();
}
