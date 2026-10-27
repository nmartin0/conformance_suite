/*
 * localeconv() in another locale: what the page requires of every locale.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.localeconv.005
 */
#include "lconv.h"

#define NN(m) pc_check(lc->m >= 0, #m " is negative (%d)", (int)lc->m)
#define PREC(m) pc_check(lc->m == 0 || lc->m == 1 || lc->m == CHAR_MAX, #m " is %d, not 0, 1 or CHAR_MAX", (int)lc->m)
#define PTR(m) pc_check(lc->m != NULL, #m " is a null pointer")

int main(void)
{
	const struct lconv *lc;

	pc_begin("XSH.localeconv.005");
	lc_init();
	lc_need();
	if (setlocale(LC_ALL, lc_loc) == NULL)
		pc_unresolved("could not select the second locale");
	pc_test();
	lc = localeconv();
	pc_check(lc != NULL, "localeconv() returned a null pointer");
	PTR(decimal_point); PTR(thousands_sep); PTR(grouping); PTR(int_curr_symbol); PTR(currency_symbol);
	PTR(mon_decimal_point); PTR(mon_thousands_sep); PTR(mon_grouping); PTR(positive_sign); PTR(negative_sign);
	pc_check(lc->decimal_point[0] != '\0', "decimal_point is empty");
	NN(int_frac_digits); NN(frac_digits); NN(p_cs_precedes); NN(p_sep_by_space); NN(n_cs_precedes);
	NN(n_sep_by_space); NN(p_sign_posn); NN(n_sign_posn); NN(int_p_cs_precedes); NN(int_p_sep_by_space);
	NN(int_n_cs_precedes); NN(int_n_sep_by_space); NN(int_p_sign_posn); NN(int_n_sign_posn);
	PREC(p_cs_precedes); PREC(n_cs_precedes); PREC(int_p_cs_precedes); PREC(int_n_cs_precedes);
	(void)setlocale(LC_ALL, "C");
	pc_pass();
}
