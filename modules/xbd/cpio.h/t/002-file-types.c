/*
 * The cpio file type values have the octal values the page gives.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.cpio.h.002
 */
#include <cpio.h>
#include "pc.h"

static const struct { const char *name; unsigned long expect; unsigned long value; int missing; } v[] = {
#ifdef C_ISDIR
	{ "C_ISDIR", 0040000, (unsigned long)(C_ISDIR), 0 },
#else
	{ "C_ISDIR", 0040000, 0, 1 },
#endif
#ifdef C_ISFIFO
	{ "C_ISFIFO", 0010000, (unsigned long)(C_ISFIFO), 0 },
#else
	{ "C_ISFIFO", 0010000, 0, 1 },
#endif
#ifdef C_ISREG
	{ "C_ISREG", 0100000, (unsigned long)(C_ISREG), 0 },
#else
	{ "C_ISREG", 0100000, 0, 1 },
#endif
#ifdef C_ISBLK
	{ "C_ISBLK", 0060000, (unsigned long)(C_ISBLK), 0 },
#else
	{ "C_ISBLK", 0060000, 0, 1 },
#endif
#ifdef C_ISCHR
	{ "C_ISCHR", 0020000, (unsigned long)(C_ISCHR), 0 },
#else
	{ "C_ISCHR", 0020000, 0, 1 },
#endif
#ifdef C_ISCTG
	{ "C_ISCTG", 0110000, (unsigned long)(C_ISCTG), 0 },
#else
	{ "C_ISCTG", 0110000, 0, 1 },
#endif
#ifdef C_ISLNK
	{ "C_ISLNK", 0120000, (unsigned long)(C_ISLNK), 0 },
#else
	{ "C_ISLNK", 0120000, 0, 1 },
#endif
#ifdef C_ISSOCK
	{ "C_ISSOCK", 0140000, (unsigned long)(C_ISSOCK), 0 },
#else
	{ "C_ISSOCK", 0140000, 0, 1 },
#endif
};

int main(void)
{
	size_t i;

	pc_begin("XBD.cpio.h.002");
	pc_test();
	for (i = 0; i < sizeof v / sizeof v[0]; i++) {
		pc_check(!v[i].missing, "the header does not define %s", v[i].name);
		pc_check(v[i].value == v[i].expect, "%s is %#lo, not %#lo", v[i].name, v[i].value, v[i].expect);
	}
	pc_pass();
}
