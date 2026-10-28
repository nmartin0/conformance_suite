/*
 * strfmon() formats with the monetary rules of the locale: the local
 * symbol, radix, grouping, fraction digits and signs for %n, and the
 * international symbol and its fraction digits for %i.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.7.3.3-lc-monetary.003
 */
#include "pc.h"
#include "ldef.h"
#include <monetary.h>

#define REST LD_CP_CTYPE LD_CP_COLLATE LD_CP_NUMERIC LD_CP_TIME LD_CP_MESSAGES

int main(void)
{
	char loc[4200], buf[200];
	ssize_t n;
	size_t l;

	pc_begin("XBD.7.3.3-lc-monetary.003");
	ld_need();
	ld_charmap("cmap", LD_HDR, '\\', "", "", 0);
	ld_ok("monfmt", "cmap",
	    "LC_MONETARY\n"
	    "int_curr_symbol \"<X><Y><Z><space>\"\ncurrency_symbol \"<Q>\"\n"
	    "mon_decimal_point \"<comma>\"\nmon_thousands_sep \"<period>\"\nmon_grouping 3\n"
	    "positive_sign \"<P>\"\nnegative_sign \"<N>\"\n"
	    "int_frac_digits 3\nfrac_digits 2\n"
	    "p_cs_precedes 1\np_sep_by_space 0\nn_cs_precedes 0\nn_sep_by_space 0\np_sign_posn 1\nn_sign_posn 2\n"
	    "int_p_cs_precedes 1\nint_p_sep_by_space 0\nint_n_cs_precedes 1\nint_n_sep_by_space 0\n"
	    "int_p_sign_posn 1\nint_n_sign_posn 1\n"
	    "END LC_MONETARY\n" REST,
	    loc, sizeof loc);
	pc_test();
	ld_load(loc);
	n = strfmon(buf, sizeof buf, "%n", 1234.5);
	pc_check(n >= 0, "strfmon %%n failed");
	if (n >= 0)
		pc_check(strcmp(buf, "PQ1.234,50") == 0, "%%n of 1234.5 gave \"%s\", not \"PQ1.234,50\"", buf);
	n = strfmon(buf, sizeof buf, "%n", -1234.5);
	pc_check(n >= 0, "strfmon %%n of a negative value failed");
	if (n >= 0)
		pc_check(strcmp(buf, "1.234,50QN") == 0, "%%n of -1234.5 gave \"%s\", not \"1.234,50QN\"", buf);
	n = strfmon(buf, sizeof buf, "%i", 1234.5);
	pc_check(n >= 0, "strfmon %%i failed");
	if (n >= 0) {
		l = strlen(buf);
		pc_check(buf[0] == 'P' && strstr(buf, "XYZ") != NULL && l >= 9 && strcmp(buf + l - 9, "1.234,500") == 0,
		    "%%i of 1234.5 gave \"%s\", which is not the sign, XYZ and 1.234,500", buf);
	}
	n = strfmon(buf, sizeof buf, "%i", -1234.5);
	pc_check(n >= 0, "strfmon %%i of a negative value failed");
	if (n >= 0) {
		l = strlen(buf);
		pc_check(buf[0] == 'N' && strstr(buf, "XYZ") != NULL && l >= 9 && strcmp(buf + l - 9, "1.234,500") == 0,
		    "%%i of -1234.5 gave \"%s\", which is not the sign, XYZ and 1.234,500", buf);
	}
	ld_unset_all();
	(void)setlocale(LC_ALL, "C");
	pc_pass();
}
