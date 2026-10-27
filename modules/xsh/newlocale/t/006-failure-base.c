/*
 * A failed call leaves the base valid and unchanged.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.newlocale.006
 */
#include "lc.h"

static const nl_item chk[] = { CODESET, DAY_1, MON_3, RADIXCHAR, YESEXPR, D_T_FMT };
#define NCHK (sizeof chk / sizeof chk[0])

int main(void)
{
	char saved[NCHK][128];
	locale_t base, r;
	int have;
	size_t i;

	pc_begin("XSH.newlocale.006");
	lc_init();
	have = lc_find();
	lc_nosuch();
	base = newlocale(LC_ALL_MASK, have ? lc_loc : "C", (locale_t)0);
	if (base == (locale_t)0)
		pc_unresolved("newlocale() could not make the base object");
	for (i = 0; i < NCHK; i++)
		lc_cp(saved[i], sizeof saved[i], nl_langinfo_l(chk[i], base));
	pc_test();
	r = newlocale(LC_TIME_MASK, LC_NOSUCH, base);
	if (r != (locale_t)0)
		pc_unresolved("newlocale() accepted the made-up locale name");
	for (i = 0; i < NCHK; i++)
		pc_check(strcmp(nl_langinfo_l(chk[i], base), saved[i]) == 0, "after the failed call item %zu of the base is \"%s\", it was \"%s\"", i, nl_langinfo_l(chk[i], base), saved[i]);
	freelocale(base);
	pc_pass();
}
