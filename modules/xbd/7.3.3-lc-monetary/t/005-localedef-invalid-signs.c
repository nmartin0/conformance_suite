/*
 * localedef does not accept a monetary category whose sign positions and
 * sign strings cannot tell negative from positive values: both positions
 * above 0 with equal or empty signs, or both positions 0, for the local
 * and for the international forms. A neighbouring valid category is
 * accepted.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.7.3.3-lc-monetary.005
 */
#include "pc.h"
#include "ldef.h"

#define REST LD_CP_CTYPE LD_CP_COLLATE LD_CP_NUMERIC LD_CP_TIME LD_CP_MESSAGES

static void bad(const char *tag, const char *what, const char *mon)
{
	char src[200], loc[4200], text[2000];
	int rc;

	(void)snprintf(text, sizeof text, "LC_MONETARY\n%sEND LC_MONETARY\n%s", mon, REST);
	(void)snprintf(src, sizeof src, "%s.src", tag);
	pc_make_file(src, text);
	ld_path(loc, sizeof loc, "bad.loc");
	rc = ld_run("", "cmap", src, loc);
	if (rc == 255)
		pc_unresolved("localedef could not be run");
	pc_check(rc != 0, "localedef accepted a category with %s", what);
}

int main(void)
{
	char ok[4200];

	pc_begin("XBD.7.3.3-lc-monetary.005");
	ld_need();
	ld_charmap("cmap", LD_HDR, '\\', "", "", 0);
	ld_ok("okpos", "cmap",
	    "LC_MONETARY\npositive_sign \"<P>\"\nnegative_sign \"<N>\"\np_sign_posn 1\nn_sign_posn 1\n"
	    "int_p_sign_posn 1\nint_n_sign_posn 1\nEND LC_MONETARY\n" REST,
	    ok, sizeof ok);
	pc_test();
	bad("eq", "equal signs and both positions above 0",
	    "positive_sign \"<P>\"\nnegative_sign \"<P>\"\np_sign_posn 1\nn_sign_posn 2\n");
	bad("empty", "empty signs and both positions above 0", "positive_sign \"\"\nnegative_sign \"\"\np_sign_posn 3\nn_sign_posn 4\n");
	bad("zero", "both sign positions 0", "positive_sign \"<P>\"\nnegative_sign \"<N>\"\np_sign_posn 0\nn_sign_posn 0\n");
	bad("ieq", "equal signs and both international positions above 0",
	    "positive_sign \"<P>\"\nnegative_sign \"<P>\"\nint_p_sign_posn 1\nint_n_sign_posn 2\n");
	bad("iempty", "empty signs and both international positions above 0",
	    "positive_sign \"\"\nnegative_sign \"\"\nint_p_sign_posn 1\nint_n_sign_posn 1\n");
	bad("izero", "both international sign positions 0",
	    "positive_sign \"<P>\"\nnegative_sign \"<N>\"\nint_p_sign_posn 0\nint_n_sign_posn 0\n");
	ld_load(ok);
	pc_check(localeconv()->p_sign_posn == 1, "the valid category was not made usable");
	ld_unset_all();
	(void)setlocale(LC_ALL, "C");
	pc_pass();
}
