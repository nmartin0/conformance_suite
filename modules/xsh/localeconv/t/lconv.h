/*
 * lconv.h - checks of struct lconv against the POSIX locale (XBD 7.3.3.1, 7.3.4.1).
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 */
#ifndef LCONV_H
#define LCONV_H

#include "lc.h"

static inline void lcv_numeric(const struct lconv *lc, const char *w)
{
	pc_check(lc != NULL, "%s: localeconv() returned a null pointer", w);
	pc_check(lc->decimal_point != NULL && strcmp(lc->decimal_point, ".") == 0, "%s: decimal_point is not \".\"", w);
	pc_check(lc->thousands_sep != NULL && lc->thousands_sep[0] == '\0', "%s: thousands_sep is not empty", w);
	pc_check(lc->grouping != NULL && lc->grouping[0] == '\0', "%s: grouping is not empty", w);
}

static inline void lcv_mstr(const struct lconv *lc, const char *w)
{
	pc_check(lc != NULL, "%s: localeconv() returned a null pointer", w);
	pc_check(lc->int_curr_symbol != NULL && lc->int_curr_symbol[0] == '\0', "%s: int_curr_symbol is not empty", w);
	pc_check(lc->currency_symbol != NULL && lc->currency_symbol[0] == '\0', "%s: currency_symbol is not empty", w);
	pc_check(lc->mon_decimal_point != NULL && lc->mon_decimal_point[0] == '\0', "%s: mon_decimal_point is not empty", w);
	pc_check(lc->mon_thousands_sep != NULL && lc->mon_thousands_sep[0] == '\0', "%s: mon_thousands_sep is not empty", w);
	pc_check(lc->mon_grouping != NULL && lc->mon_grouping[0] == '\0', "%s: mon_grouping is not empty", w);
	pc_check(lc->positive_sign != NULL && lc->positive_sign[0] == '\0', "%s: positive_sign is not empty", w);
	pc_check(lc->negative_sign != NULL && lc->negative_sign[0] == '\0', "%s: negative_sign is not empty", w);
}

#define LCV_CM(m) pc_check(lc->m == CHAR_MAX, "%s: " #m " is %d, expected CHAR_MAX", w, (int)lc->m)

static inline void lcv_mchar(const struct lconv *lc, const char *w)
{
	pc_check(lc != NULL, "%s: localeconv() returned a null pointer", w);
	LCV_CM(int_frac_digits);
	LCV_CM(frac_digits);
	LCV_CM(p_cs_precedes);
	LCV_CM(p_sep_by_space);
	LCV_CM(n_cs_precedes);
	LCV_CM(n_sep_by_space);
	LCV_CM(p_sign_posn);
	LCV_CM(n_sign_posn);
	LCV_CM(int_p_cs_precedes);
	LCV_CM(int_p_sep_by_space);
	LCV_CM(int_n_cs_precedes);
	LCV_CM(int_n_sep_by_space);
	LCV_CM(int_p_sign_posn);
	LCV_CM(int_n_sign_posn);
}

#endif
