/*
 * Every LC_MONETARY keyword given to localedef reaches the member of the
 * localeconv() result that bears its name, with its value intact.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.7.3.3-lc-monetary.001
 */
#include "pc.h"
#include "ldef.h"

#define REST LD_CP_CTYPE LD_CP_COLLATE LD_CP_NUMERIC LD_CP_TIME LD_CP_MESSAGES

int main(void)
{
	char loc[4200];
	const struct lconv *lc;

	pc_begin("XBD.7.3.3-lc-monetary.001");
	ld_need();
	ld_charmap("cmap", LD_HDR, '\\', "", "", 0);
	ld_ok("mon", "cmap",
	    "LC_MONETARY\n"
	    "int_curr_symbol \"<X><Y><Z><space>\"\n"
	    "currency_symbol \"<Q><q>\"\n"
	    "mon_decimal_point \"<comma>\"\n"
	    "mon_thousands_sep \"<period>\"\n"
	    "mon_grouping 3;2\n"
	    "positive_sign \"<P>\"\n"
	    "negative_sign \"<N>\"\n"
	    "int_frac_digits 3\nfrac_digits 1\n"
	    "p_cs_precedes 1\np_sep_by_space 2\nn_cs_precedes 0\nn_sep_by_space 1\n"
	    "p_sign_posn 3\nn_sign_posn 4\n"
	    "int_p_cs_precedes 0\nint_p_sep_by_space 1\nint_n_cs_precedes 1\nint_n_sep_by_space 2\n"
	    "int_p_sign_posn 2\nint_n_sign_posn 1\n"
	    "END LC_MONETARY\n" REST,
	    loc, sizeof loc);
	pc_test();
	ld_load(loc);
	lc = localeconv();
	pc_check(lc != NULL, "localeconv returned a null pointer");
	pc_check(strcmp(lc->int_curr_symbol, "XYZ ") == 0, "int_curr_symbol is \"%s\", not \"XYZ \"", lc->int_curr_symbol);
	pc_check(strcmp(lc->currency_symbol, "Qq") == 0, "currency_symbol is \"%s\", not \"Qq\"", lc->currency_symbol);
	pc_check(strcmp(lc->mon_decimal_point, ",") == 0, "mon_decimal_point is \"%s\", not \",\"", lc->mon_decimal_point);
	pc_check(strcmp(lc->mon_thousands_sep, ".") == 0, "mon_thousands_sep is \"%s\", not \".\"", lc->mon_thousands_sep);
	pc_check(strcmp(lc->mon_grouping, "\3\2") == 0, "mon_grouping is not the groups 3 and 2");
	pc_check(strcmp(lc->positive_sign, "P") == 0, "positive_sign is \"%s\", not \"P\"", lc->positive_sign);
	pc_check(strcmp(lc->negative_sign, "N") == 0, "negative_sign is \"%s\", not \"N\"", lc->negative_sign);
	pc_check(lc->int_frac_digits == 3, "int_frac_digits is %d, not 3", lc->int_frac_digits);
	pc_check(lc->frac_digits == 1, "frac_digits is %d, not 1", lc->frac_digits);
	pc_check(lc->p_cs_precedes == 1, "p_cs_precedes is %d, not 1", lc->p_cs_precedes);
	pc_check(lc->p_sep_by_space == 2, "p_sep_by_space is %d, not 2", lc->p_sep_by_space);
	pc_check(lc->n_cs_precedes == 0, "n_cs_precedes is %d, not 0", lc->n_cs_precedes);
	pc_check(lc->n_sep_by_space == 1, "n_sep_by_space is %d, not 1", lc->n_sep_by_space);
	pc_check(lc->p_sign_posn == 3, "p_sign_posn is %d, not 3", lc->p_sign_posn);
	pc_check(lc->n_sign_posn == 4, "n_sign_posn is %d, not 4", lc->n_sign_posn);
	pc_check(lc->int_p_cs_precedes == 0, "int_p_cs_precedes is %d, not 0", lc->int_p_cs_precedes);
	pc_check(lc->int_p_sep_by_space == 1, "int_p_sep_by_space is %d, not 1", lc->int_p_sep_by_space);
	pc_check(lc->int_n_cs_precedes == 1, "int_n_cs_precedes is %d, not 1", lc->int_n_cs_precedes);
	pc_check(lc->int_n_sep_by_space == 2, "int_n_sep_by_space is %d, not 2", lc->int_n_sep_by_space);
	pc_check(lc->int_p_sign_posn == 2, "int_p_sign_posn is %d, not 2", lc->int_p_sign_posn);
	pc_check(lc->int_n_sign_posn == 1, "int_n_sign_posn is %d, not 1", lc->int_n_sign_posn);
	ld_unset_all();
	(void)setlocale(LC_ALL, "C");
	pc_pass();
}
