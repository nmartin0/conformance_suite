/*
 * The regcomp() flags REG_EXTENDED, REG_ICASE, REG_NOSUB and REG_NEWLINE
 *   are non-zero with no bit in common.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.regex.h.001
 */
#include <regex.h>
#include "pc.h"


/* name, value, missing */
static const struct { const char *name; unsigned long long value; int missing; } f[] = {
#ifdef REG_EXTENDED
	{ "REG_EXTENDED", (unsigned long long)(REG_EXTENDED), 0 },
#else
	{ "REG_EXTENDED", 0, 1 },
#endif
#ifdef REG_ICASE
	{ "REG_ICASE", (unsigned long long)(REG_ICASE), 0 },
#else
	{ "REG_ICASE", 0, 1 },
#endif
#ifdef REG_NOSUB
	{ "REG_NOSUB", (unsigned long long)(REG_NOSUB), 0 },
#else
	{ "REG_NOSUB", 0, 1 },
#endif
#ifdef REG_NEWLINE
	{ "REG_NEWLINE", (unsigned long long)(REG_NEWLINE), 0 },
#else
	{ "REG_NEWLINE", 0, 1 },
#endif
};

int main(void)
{
	size_t i, j, n = sizeof f / sizeof f[0];

	pc_begin("XBD.regex.h.001");
	pc_test();
	for (i = 0; i < n; i++) {
		pc_check(!f[i].missing, "<regex.h> does not define %s", f[i].name);
		pc_check(f[i].value != 0, "%s is 0", f[i].name);
		for (j = i + 1; j < n; j++)
			pc_check(f[j].missing || (f[i].value & f[j].value) == 0, "%s (%#llx) and %s (%#llx) have bits in common",
			    f[i].name, f[i].value, f[j].name, f[j].value);
	}
	pc_pass();
}
