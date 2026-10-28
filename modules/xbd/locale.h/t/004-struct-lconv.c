/*
 * <locale.h> struct lconv
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.locale.h.004
 *
 * Only <locale.h> is included (not pc.h, which brings in <stddef.h>), so a name
 * the header fails to provide stops the build, which the module reports as FAIL.
 */
#include <locale.h>

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

typedef char pc_c0[sizeof(struct lconv)];	/* a complete type */
static struct lconv *ps;

int main(void)
{
	pc_begin("XBD.locale.h.004");
	pc_test();
	(void)ps;
	if (!IS_T(&ps->currency_symbol, char * *))
		pc_fail("struct lconv member currency_symbol is not of type char *");
	if (!IS_T(&ps->decimal_point, char * *))
		pc_fail("struct lconv member decimal_point is not of type char *");
	if (!IS_T(&ps->frac_digits, char *))
		pc_fail("struct lconv member frac_digits is not of type char");
	if (!IS_T(&ps->grouping, char * *))
		pc_fail("struct lconv member grouping is not of type char *");
	if (!IS_T(&ps->int_curr_symbol, char * *))
		pc_fail("struct lconv member int_curr_symbol is not of type char *");
	if (!IS_T(&ps->int_frac_digits, char *))
		pc_fail("struct lconv member int_frac_digits is not of type char");
	if (!IS_T(&ps->int_n_cs_precedes, char *))
		pc_fail("struct lconv member int_n_cs_precedes is not of type char");
	if (!IS_T(&ps->int_n_sep_by_space, char *))
		pc_fail("struct lconv member int_n_sep_by_space is not of type char");
	if (!IS_T(&ps->int_n_sign_posn, char *))
		pc_fail("struct lconv member int_n_sign_posn is not of type char");
	if (!IS_T(&ps->int_p_cs_precedes, char *))
		pc_fail("struct lconv member int_p_cs_precedes is not of type char");
	if (!IS_T(&ps->int_p_sep_by_space, char *))
		pc_fail("struct lconv member int_p_sep_by_space is not of type char");
	if (!IS_T(&ps->int_p_sign_posn, char *))
		pc_fail("struct lconv member int_p_sign_posn is not of type char");
	if (!IS_T(&ps->mon_decimal_point, char * *))
		pc_fail("struct lconv member mon_decimal_point is not of type char *");
	if (!IS_T(&ps->mon_grouping, char * *))
		pc_fail("struct lconv member mon_grouping is not of type char *");
	if (!IS_T(&ps->mon_thousands_sep, char * *))
		pc_fail("struct lconv member mon_thousands_sep is not of type char *");
	if (!IS_T(&ps->negative_sign, char * *))
		pc_fail("struct lconv member negative_sign is not of type char *");
	if (!IS_T(&ps->n_cs_precedes, char *))
		pc_fail("struct lconv member n_cs_precedes is not of type char");
	if (!IS_T(&ps->n_sep_by_space, char *))
		pc_fail("struct lconv member n_sep_by_space is not of type char");
	if (!IS_T(&ps->n_sign_posn, char *))
		pc_fail("struct lconv member n_sign_posn is not of type char");
	if (!IS_T(&ps->positive_sign, char * *))
		pc_fail("struct lconv member positive_sign is not of type char *");
	if (!IS_T(&ps->p_cs_precedes, char *))
		pc_fail("struct lconv member p_cs_precedes is not of type char");
	if (!IS_T(&ps->p_sep_by_space, char *))
		pc_fail("struct lconv member p_sep_by_space is not of type char");
	if (!IS_T(&ps->p_sign_posn, char *))
		pc_fail("struct lconv member p_sign_posn is not of type char");
	if (!IS_T(&ps->thousands_sep, char * *))
		pc_fail("struct lconv member thousands_sep is not of type char *");
	pc_pass();
}
