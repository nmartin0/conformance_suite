/*
 * The thirteen REG_ error codes are non-zero and all different.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.regex.h.004
 */
#include <regex.h>
#include "pc.h"


static const struct { const char *name; long value; int missing; } e[] = {
#ifdef REG_NOMATCH
	{ "REG_NOMATCH", (long)(REG_NOMATCH), 0 },
#else
	{ "REG_NOMATCH", 0, 1 },
#endif
#ifdef REG_BADPAT
	{ "REG_BADPAT", (long)(REG_BADPAT), 0 },
#else
	{ "REG_BADPAT", 0, 1 },
#endif
#ifdef REG_ECOLLATE
	{ "REG_ECOLLATE", (long)(REG_ECOLLATE), 0 },
#else
	{ "REG_ECOLLATE", 0, 1 },
#endif
#ifdef REG_ECTYPE
	{ "REG_ECTYPE", (long)(REG_ECTYPE), 0 },
#else
	{ "REG_ECTYPE", 0, 1 },
#endif
#ifdef REG_EESCAPE
	{ "REG_EESCAPE", (long)(REG_EESCAPE), 0 },
#else
	{ "REG_EESCAPE", 0, 1 },
#endif
#ifdef REG_ESUBREG
	{ "REG_ESUBREG", (long)(REG_ESUBREG), 0 },
#else
	{ "REG_ESUBREG", 0, 1 },
#endif
#ifdef REG_EBRACK
	{ "REG_EBRACK", (long)(REG_EBRACK), 0 },
#else
	{ "REG_EBRACK", 0, 1 },
#endif
#ifdef REG_EPAREN
	{ "REG_EPAREN", (long)(REG_EPAREN), 0 },
#else
	{ "REG_EPAREN", 0, 1 },
#endif
#ifdef REG_EBRACE
	{ "REG_EBRACE", (long)(REG_EBRACE), 0 },
#else
	{ "REG_EBRACE", 0, 1 },
#endif
#ifdef REG_BADBR
	{ "REG_BADBR", (long)(REG_BADBR), 0 },
#else
	{ "REG_BADBR", 0, 1 },
#endif
#ifdef REG_ERANGE
	{ "REG_ERANGE", (long)(REG_ERANGE), 0 },
#else
	{ "REG_ERANGE", 0, 1 },
#endif
#ifdef REG_ESPACE
	{ "REG_ESPACE", (long)(REG_ESPACE), 0 },
#else
	{ "REG_ESPACE", 0, 1 },
#endif
#ifdef REG_BADRPT
	{ "REG_BADRPT", (long)(REG_BADRPT), 0 },
#else
	{ "REG_BADRPT", 0, 1 },
#endif
};

int main(void)
{
	size_t i, j, n = sizeof e / sizeof e[0];

	pc_begin("XBD.regex.h.004");
	pc_test();
	for (i = 0; i < n; i++) {
		pc_check(!e[i].missing, "<regex.h> does not define %s", e[i].name);
		pc_check(e[i].value != 0, "%s is 0, the success value", e[i].name);
		for (j = i + 1; j < n; j++)
			pc_check(e[i].value != e[j].value, "%s and %s are both %ld", e[i].name, e[j].name, e[i].value);
	}
	pc_pass();
}
