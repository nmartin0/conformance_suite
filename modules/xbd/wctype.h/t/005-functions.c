/*
 * <wctype.h> functions
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.wctype.h.005
 *
 * Only <wctype.h> is included (not pc.h, which brings in <stddef.h>), so a name
 * the header fails to provide stops the build, which the module reports as FAIL.
 */
#include <wctype.h>

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

int main(void)
{
	pc_begin("XBD.wctype.h.005");
	pc_test();
	if (!IS_T(&(iswalnum), int (*)(wint_t)))
		pc_fail("iswalnum() is not declared as int iswalnum(wint_t)");
	if (IS_T(&(iswalnum), int (*)(wint_t, int)))
		pc_fail("iswalnum() is declared without a prototype");
	if (!IS_T(&(iswalnum_l), int (*)(wint_t, locale_t)))
		pc_fail("iswalnum_l() is not declared as int iswalnum_l(wint_t, locale_t)");
	if (IS_T(&(iswalnum_l), int (*)(wint_t, locale_t, int)))
		pc_fail("iswalnum_l() is declared without a prototype");
	if (!IS_T(&(iswalpha), int (*)(wint_t)))
		pc_fail("iswalpha() is not declared as int iswalpha(wint_t)");
	if (IS_T(&(iswalpha), int (*)(wint_t, int)))
		pc_fail("iswalpha() is declared without a prototype");
	if (!IS_T(&(iswalpha_l), int (*)(wint_t, locale_t)))
		pc_fail("iswalpha_l() is not declared as int iswalpha_l(wint_t, locale_t)");
	if (IS_T(&(iswalpha_l), int (*)(wint_t, locale_t, int)))
		pc_fail("iswalpha_l() is declared without a prototype");
	if (!IS_T(&(iswblank), int (*)(wint_t)))
		pc_fail("iswblank() is not declared as int iswblank(wint_t)");
	if (IS_T(&(iswblank), int (*)(wint_t, int)))
		pc_fail("iswblank() is declared without a prototype");
	if (!IS_T(&(iswblank_l), int (*)(wint_t, locale_t)))
		pc_fail("iswblank_l() is not declared as int iswblank_l(wint_t, locale_t)");
	if (IS_T(&(iswblank_l), int (*)(wint_t, locale_t, int)))
		pc_fail("iswblank_l() is declared without a prototype");
	if (!IS_T(&(iswcntrl), int (*)(wint_t)))
		pc_fail("iswcntrl() is not declared as int iswcntrl(wint_t)");
	if (IS_T(&(iswcntrl), int (*)(wint_t, int)))
		pc_fail("iswcntrl() is declared without a prototype");
	if (!IS_T(&(iswcntrl_l), int (*)(wint_t, locale_t)))
		pc_fail("iswcntrl_l() is not declared as int iswcntrl_l(wint_t, locale_t)");
	if (IS_T(&(iswcntrl_l), int (*)(wint_t, locale_t, int)))
		pc_fail("iswcntrl_l() is declared without a prototype");
	if (!IS_T(&(iswctype), int (*)(wint_t, wctype_t)))
		pc_fail("iswctype() is not declared as int iswctype(wint_t, wctype_t)");
	if (IS_T(&(iswctype), int (*)(wint_t, wctype_t, int)))
		pc_fail("iswctype() is declared without a prototype");
	if (!IS_T(&(iswctype_l), int (*)(wint_t, wctype_t, locale_t)))
		pc_fail("iswctype_l() is not declared as int iswctype_l(wint_t, wctype_t, locale_t)");
	if (IS_T(&(iswctype_l), int (*)(wint_t, wctype_t, locale_t, int)))
		pc_fail("iswctype_l() is declared without a prototype");
	if (!IS_T(&(iswdigit), int (*)(wint_t)))
		pc_fail("iswdigit() is not declared as int iswdigit(wint_t)");
	if (IS_T(&(iswdigit), int (*)(wint_t, int)))
		pc_fail("iswdigit() is declared without a prototype");
	if (!IS_T(&(iswdigit_l), int (*)(wint_t, locale_t)))
		pc_fail("iswdigit_l() is not declared as int iswdigit_l(wint_t, locale_t)");
	if (IS_T(&(iswdigit_l), int (*)(wint_t, locale_t, int)))
		pc_fail("iswdigit_l() is declared without a prototype");
	if (!IS_T(&(iswgraph), int (*)(wint_t)))
		pc_fail("iswgraph() is not declared as int iswgraph(wint_t)");
	if (IS_T(&(iswgraph), int (*)(wint_t, int)))
		pc_fail("iswgraph() is declared without a prototype");
	if (!IS_T(&(iswgraph_l), int (*)(wint_t, locale_t)))
		pc_fail("iswgraph_l() is not declared as int iswgraph_l(wint_t, locale_t)");
	if (IS_T(&(iswgraph_l), int (*)(wint_t, locale_t, int)))
		pc_fail("iswgraph_l() is declared without a prototype");
	if (!IS_T(&(iswlower), int (*)(wint_t)))
		pc_fail("iswlower() is not declared as int iswlower(wint_t)");
	if (IS_T(&(iswlower), int (*)(wint_t, int)))
		pc_fail("iswlower() is declared without a prototype");
	if (!IS_T(&(iswlower_l), int (*)(wint_t, locale_t)))
		pc_fail("iswlower_l() is not declared as int iswlower_l(wint_t, locale_t)");
	if (IS_T(&(iswlower_l), int (*)(wint_t, locale_t, int)))
		pc_fail("iswlower_l() is declared without a prototype");
	if (!IS_T(&(iswprint), int (*)(wint_t)))
		pc_fail("iswprint() is not declared as int iswprint(wint_t)");
	if (IS_T(&(iswprint), int (*)(wint_t, int)))
		pc_fail("iswprint() is declared without a prototype");
	if (!IS_T(&(iswprint_l), int (*)(wint_t, locale_t)))
		pc_fail("iswprint_l() is not declared as int iswprint_l(wint_t, locale_t)");
	if (IS_T(&(iswprint_l), int (*)(wint_t, locale_t, int)))
		pc_fail("iswprint_l() is declared without a prototype");
	if (!IS_T(&(iswpunct), int (*)(wint_t)))
		pc_fail("iswpunct() is not declared as int iswpunct(wint_t)");
	if (IS_T(&(iswpunct), int (*)(wint_t, int)))
		pc_fail("iswpunct() is declared without a prototype");
	if (!IS_T(&(iswpunct_l), int (*)(wint_t, locale_t)))
		pc_fail("iswpunct_l() is not declared as int iswpunct_l(wint_t, locale_t)");
	if (IS_T(&(iswpunct_l), int (*)(wint_t, locale_t, int)))
		pc_fail("iswpunct_l() is declared without a prototype");
	if (!IS_T(&(iswspace), int (*)(wint_t)))
		pc_fail("iswspace() is not declared as int iswspace(wint_t)");
	if (IS_T(&(iswspace), int (*)(wint_t, int)))
		pc_fail("iswspace() is declared without a prototype");
	if (!IS_T(&(iswspace_l), int (*)(wint_t, locale_t)))
		pc_fail("iswspace_l() is not declared as int iswspace_l(wint_t, locale_t)");
	if (IS_T(&(iswspace_l), int (*)(wint_t, locale_t, int)))
		pc_fail("iswspace_l() is declared without a prototype");
	if (!IS_T(&(iswupper), int (*)(wint_t)))
		pc_fail("iswupper() is not declared as int iswupper(wint_t)");
	if (IS_T(&(iswupper), int (*)(wint_t, int)))
		pc_fail("iswupper() is declared without a prototype");
	if (!IS_T(&(iswupper_l), int (*)(wint_t, locale_t)))
		pc_fail("iswupper_l() is not declared as int iswupper_l(wint_t, locale_t)");
	if (IS_T(&(iswupper_l), int (*)(wint_t, locale_t, int)))
		pc_fail("iswupper_l() is declared without a prototype");
	if (!IS_T(&(iswxdigit), int (*)(wint_t)))
		pc_fail("iswxdigit() is not declared as int iswxdigit(wint_t)");
	if (IS_T(&(iswxdigit), int (*)(wint_t, int)))
		pc_fail("iswxdigit() is declared without a prototype");
	if (!IS_T(&(iswxdigit_l), int (*)(wint_t, locale_t)))
		pc_fail("iswxdigit_l() is not declared as int iswxdigit_l(wint_t, locale_t)");
	if (IS_T(&(iswxdigit_l), int (*)(wint_t, locale_t, int)))
		pc_fail("iswxdigit_l() is declared without a prototype");
	if (!IS_T(&(towctrans), wint_t (*)(wint_t, wctrans_t)))
		pc_fail("towctrans() is not declared as wint_t towctrans(wint_t, wctrans_t)");
	if (IS_T(&(towctrans), wint_t (*)(wint_t, wctrans_t, int)))
		pc_fail("towctrans() is declared without a prototype");
	if (!IS_T(&(towctrans_l), wint_t (*)(wint_t, wctrans_t, locale_t)))
		pc_fail("towctrans_l() is not declared as wint_t towctrans_l(wint_t, wctrans_t, locale_t)");
	if (IS_T(&(towctrans_l), wint_t (*)(wint_t, wctrans_t, locale_t, int)))
		pc_fail("towctrans_l() is declared without a prototype");
	if (!IS_T(&(towlower), wint_t (*)(wint_t)))
		pc_fail("towlower() is not declared as wint_t towlower(wint_t)");
	if (IS_T(&(towlower), wint_t (*)(wint_t, int)))
		pc_fail("towlower() is declared without a prototype");
	if (!IS_T(&(towlower_l), wint_t (*)(wint_t, locale_t)))
		pc_fail("towlower_l() is not declared as wint_t towlower_l(wint_t, locale_t)");
	if (IS_T(&(towlower_l), wint_t (*)(wint_t, locale_t, int)))
		pc_fail("towlower_l() is declared without a prototype");
	if (!IS_T(&(towupper), wint_t (*)(wint_t)))
		pc_fail("towupper() is not declared as wint_t towupper(wint_t)");
	if (IS_T(&(towupper), wint_t (*)(wint_t, int)))
		pc_fail("towupper() is declared without a prototype");
	if (!IS_T(&(towupper_l), wint_t (*)(wint_t, locale_t)))
		pc_fail("towupper_l() is not declared as wint_t towupper_l(wint_t, locale_t)");
	if (IS_T(&(towupper_l), wint_t (*)(wint_t, locale_t, int)))
		pc_fail("towupper_l() is declared without a prototype");
	if (!IS_T(&(wctrans), wctrans_t (*)(const char *)))
		pc_fail("wctrans() is not declared as wctrans_t wctrans(const char *)");
	if (IS_T(&(wctrans), wctrans_t (*)(const char *, int)))
		pc_fail("wctrans() is declared without a prototype");
	if (!IS_T(&(wctrans_l), wctrans_t (*)(const char *, locale_t)))
		pc_fail("wctrans_l() is not declared as wctrans_t wctrans_l(const char *, locale_t)");
	if (IS_T(&(wctrans_l), wctrans_t (*)(const char *, locale_t, int)))
		pc_fail("wctrans_l() is declared without a prototype");
	if (!IS_T(&(wctype), wctype_t (*)(const char *)))
		pc_fail("wctype() is not declared as wctype_t wctype(const char *)");
	if (IS_T(&(wctype), wctype_t (*)(const char *, int)))
		pc_fail("wctype() is declared without a prototype");
	if (!IS_T(&(wctype_l), wctype_t (*)(const char *, locale_t)))
		pc_fail("wctype_l() is not declared as wctype_t wctype_l(const char *, locale_t)");
	if (IS_T(&(wctype_l), wctype_t (*)(const char *, locale_t, int)))
		pc_fail("wctype_l() is declared without a prototype");
	pc_pass();
}
