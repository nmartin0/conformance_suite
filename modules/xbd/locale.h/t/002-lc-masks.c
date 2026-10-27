/*
 * The LC_ _MASK macros are non-zero bit sets with no bit in common, and
 *   LC_ALL_MASK holds all of them.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.locale.h.002
 */
#include <locale.h>
#include "pc.h"


static const struct { const char *name; unsigned long long value; } m[] = {
#ifdef LC_COLLATE_MASK
	{ "LC_COLLATE_MASK", (unsigned long long)(LC_COLLATE_MASK) },
#else
	{ "LC_COLLATE_MASK (missing)", 0 },
#endif
#ifdef LC_CTYPE_MASK
	{ "LC_CTYPE_MASK", (unsigned long long)(LC_CTYPE_MASK) },
#else
	{ "LC_CTYPE_MASK (missing)", 0 },
#endif
#ifdef LC_MESSAGES_MASK
	{ "LC_MESSAGES_MASK", (unsigned long long)(LC_MESSAGES_MASK) },
#else
	{ "LC_MESSAGES_MASK (missing)", 0 },
#endif
#ifdef LC_MONETARY_MASK
	{ "LC_MONETARY_MASK", (unsigned long long)(LC_MONETARY_MASK) },
#else
	{ "LC_MONETARY_MASK (missing)", 0 },
#endif
#ifdef LC_NUMERIC_MASK
	{ "LC_NUMERIC_MASK", (unsigned long long)(LC_NUMERIC_MASK) },
#else
	{ "LC_NUMERIC_MASK (missing)", 0 },
#endif
#ifdef LC_TIME_MASK
	{ "LC_TIME_MASK", (unsigned long long)(LC_TIME_MASK) },
#else
	{ "LC_TIME_MASK (missing)", 0 },
#endif
};

int main(void)
{
	size_t i, j, n = sizeof m / sizeof m[0];

	pc_begin("XBD.locale.h.002");
	pc_test();
#if !defined(LC_ALL_MASK)
	pc_fail("<locale.h> does not define LC_ALL_MASK");
#else
	for (i = 0; i < n; i++) {
		pc_check(m[i].value != 0, "%s is missing or zero", m[i].name);
		pc_check((m[i].value & (unsigned long long)LC_ALL_MASK) == m[i].value, "LC_ALL_MASK (%#llx) lacks the bits of %s (%#llx)",
		    (unsigned long long)LC_ALL_MASK, m[i].name, m[i].value);
		for (j = i + 1; j < n; j++)
			pc_check((m[i].value & m[j].value) == 0, "%s (%#llx) and %s (%#llx) have bits in common",
			    m[i].name, m[i].value, m[j].name, m[j].value);
	}
	pc_pass();
#endif
}
