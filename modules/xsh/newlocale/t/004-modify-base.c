/*
 * Modifying a base object replaces only the masked categories.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.newlocale.004
 */
#include "lc.h"

static const nl_item chk[] = { DAY_1, DAY_7, ABDAY_4, MON_1, MON_12, ABMON_6, AM_STR, D_T_FMT, D_FMT, T_FMT, RADIXCHAR, YESEXPR, NOEXPR };
#define NCHK (sizeof chk / sizeof chk[0])

int main(void)
{
	char saved[NCHK][128];
	locale_t base, r;
	size_t i;

	pc_begin("XSH.newlocale.004");
	lc_init();
	lc_need();
	base = newlocale(LC_ALL_MASK, lc_loc, (locale_t)0);
	if (base == (locale_t)0)
		pc_unresolved("newlocale() could not make an object from the second locale");
	for (i = 0; i < NCHK; i++)
		lc_cp(saved[i], sizeof saved[i], nl_langinfo_l(chk[i], base));
	pc_test();
	r = newlocale(LC_CTYPE_MASK, "C", base);
	pc_check(r != (locale_t)0, "newlocale(LC_CTYPE_MASK, \"C\", base) failed, errno %s", pc_errname(errno));
	pc_check(strcmp(nl_langinfo_l(CODESET, r), lc_c0) == 0, "the code set of the result is \"%s\", expected the POSIX one \"%s\"", nl_langinfo_l(CODESET, r), lc_c0);
	for (i = 0; i < NCHK; i++)
		pc_check(strcmp(nl_langinfo_l(chk[i], r), saved[i]) == 0, "item %zu of the result is \"%s\", the base had \"%s\"", i, nl_langinfo_l(chk[i], r), saved[i]);
	freelocale(r);
	pc_pass();
}
