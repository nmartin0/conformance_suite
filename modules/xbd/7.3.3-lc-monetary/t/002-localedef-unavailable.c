/*
 * Empty strings and integer keywords set to -1 are unavailable items:
 * localeconv() gives the empty string for the strings and {CHAR_MAX} for
 * the integers.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.7.3.3-lc-monetary.002
 */
#include "pc.h"
#include "ldef.h"
#include <limits.h>

#define REST LD_CP_CTYPE LD_CP_COLLATE LD_CP_NUMERIC LD_CP_TIME LD_CP_MESSAGES

int main(void)
{
	char loc[4200];
	const struct lconv *lc;

	pc_begin("XBD.7.3.3-lc-monetary.002");
	ld_need();
	ld_charmap("cmap", LD_HDR, '\\', "", "", 0);
	ld_ok("monun", "cmap",
	    "LC_MONETARY\n"
	    "int_curr_symbol \"\"\ncurrency_symbol \"\"\nmon_decimal_point \"\"\nmon_thousands_sep \"\"\n"
	    "positive_sign \"\"\nnegative_sign \"\"\n"
	    "int_frac_digits -1\nfrac_digits -1\n"
	    "p_cs_precedes -1\np_sep_by_space -1\nn_cs_precedes -1\nn_sep_by_space -1\n"
	    "p_sign_posn -1\nn_sign_posn -1\n"
	    "int_p_cs_precedes -1\nint_p_sep_by_space -1\nint_n_cs_precedes -1\nint_n_sep_by_space -1\n"
	    "int_p_sign_posn -1\nint_n_sign_posn -1\n"
	    "END LC_MONETARY\n" REST,
	    loc, sizeof loc);
	pc_test();
	ld_load(loc);
	lc = localeconv();
	pc_check(lc != NULL, "localeconv returned a null pointer");
	pc_check(lc->int_curr_symbol[0] == '\0', "int_curr_symbol is not empty");
	pc_check(lc->currency_symbol[0] == '\0', "currency_symbol is not empty");
	pc_check(lc->mon_decimal_point[0] == '\0', "mon_decimal_point is not empty");
	pc_check(lc->mon_thousands_sep[0] == '\0', "mon_thousands_sep is not empty");
	pc_check(lc->positive_sign[0] == '\0', "positive_sign is not empty");
	pc_check(lc->negative_sign[0] == '\0', "negative_sign is not empty");
	pc_check(lc->int_frac_digits == CHAR_MAX, "int_frac_digits is %d, not CHAR_MAX", lc->int_frac_digits);
	pc_check(lc->frac_digits == CHAR_MAX, "frac_digits is %d, not CHAR_MAX", lc->frac_digits);
	pc_check(lc->p_cs_precedes == CHAR_MAX, "p_cs_precedes is %d, not CHAR_MAX", lc->p_cs_precedes);
	pc_check(lc->p_sep_by_space == CHAR_MAX, "p_sep_by_space is %d, not CHAR_MAX", lc->p_sep_by_space);
	pc_check(lc->n_cs_precedes == CHAR_MAX, "n_cs_precedes is %d, not CHAR_MAX", lc->n_cs_precedes);
	pc_check(lc->n_sep_by_space == CHAR_MAX, "n_sep_by_space is %d, not CHAR_MAX", lc->n_sep_by_space);
	pc_check(lc->p_sign_posn == CHAR_MAX, "p_sign_posn is %d, not CHAR_MAX", lc->p_sign_posn);
	pc_check(lc->n_sign_posn == CHAR_MAX, "n_sign_posn is %d, not CHAR_MAX", lc->n_sign_posn);
	pc_check(lc->int_p_cs_precedes == CHAR_MAX, "int_p_cs_precedes is %d, not CHAR_MAX", lc->int_p_cs_precedes);
	pc_check(lc->int_p_sep_by_space == CHAR_MAX, "int_p_sep_by_space is %d, not CHAR_MAX", lc->int_p_sep_by_space);
	pc_check(lc->int_n_cs_precedes == CHAR_MAX, "int_n_cs_precedes is %d, not CHAR_MAX", lc->int_n_cs_precedes);
	pc_check(lc->int_n_sep_by_space == CHAR_MAX, "int_n_sep_by_space is %d, not CHAR_MAX", lc->int_n_sep_by_space);
	pc_check(lc->int_p_sign_posn == CHAR_MAX, "int_p_sign_posn is %d, not CHAR_MAX", lc->int_p_sign_posn);
	pc_check(lc->int_n_sign_posn == CHAR_MAX, "int_n_sign_posn is %d, not CHAR_MAX", lc->int_n_sign_posn);
	ld_unset_all();
	(void)setlocale(LC_ALL, "C");
	pc_pass();
}
