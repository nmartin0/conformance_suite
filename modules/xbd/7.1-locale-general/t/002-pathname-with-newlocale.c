/*
 * newlocale() with the empty locale name takes the locale from the
 * environment in the same way: with LC_ALL naming the pathname of a locale
 * made by localedef the object is made and carries the locale's data.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.7.1-locale-general.002
 */
#include "pc.h"
#include "ldef.h"
#include <langinfo.h>

#define SRC LD_CP_CTYPE LD_CP_COLLATE LD_CP_MONETARY LD_CP_TIME LD_CP_MESSAGES \
	"LC_NUMERIC\ndecimal_point \",\"\nthousands_sep \".\"\ngrouping 3;3\nEND LC_NUMERIC\n"

int main(void)
{
	char loc[4200];
	locale_t l;

	pc_begin("XBD.7.1-locale-general.002");
	ld_need();
	ld_ok("pathloc", NULL, SRC, loc, sizeof loc);
	ld_unset_all();
	pc_check(setenv("LC_ALL", loc, 1) == 0, "setenv failed");
	pc_test();
	errno = 0;
	l = newlocale(LC_ALL_MASK, "", (locale_t)0);
	pc_check(l != (locale_t)0, "newlocale(LC_ALL_MASK, \"\") failed (%s) with LC_ALL naming the locale pathname %s",
	    pc_errname(errno), loc);
	pc_check(strcmp(nl_langinfo_l(RADIXCHAR, l), ",") == 0, "the radix character of the object is \"%s\", not \",\"",
	    nl_langinfo_l(RADIXCHAR, l));
	pc_check(strcmp(nl_langinfo_l(THOUSEP, l), ".") == 0, "the grouping separator of the object is \"%s\", not \".\"",
	    nl_langinfo_l(THOUSEP, l));
	freelocale(l);
	ld_unset_all();
	pc_pass();
}
