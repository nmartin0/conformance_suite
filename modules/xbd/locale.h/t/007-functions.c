/*
 * <locale.h> functions
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.locale.h.007
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

int main(void)
{
	pc_begin("XBD.locale.h.007");
	pc_test();
	if (!IS_T(&(duplocale), locale_t (*)(locale_t)))
		pc_fail("duplocale() is not declared as locale_t duplocale(locale_t)");
	if (IS_T(&(duplocale), locale_t (*)(locale_t, int)))
		pc_fail("duplocale() is declared without a prototype");
	if (!IS_T(&(freelocale), void (*)(locale_t)))
		pc_fail("freelocale() is not declared as void freelocale(locale_t)");
	if (IS_T(&(freelocale), void (*)(locale_t, int)))
		pc_fail("freelocale() is declared without a prototype");
	if (!IS_T(&(localeconv), struct lconv *(*)(void)))
		pc_fail("localeconv() is not declared as struct lconv *localeconv(void)");
	if (IS_T(&(localeconv), struct lconv *(*)(int)))
		pc_fail("localeconv() is declared without a prototype");
	if (!IS_T(&(newlocale), locale_t (*)(int, const char *, locale_t)))
		pc_fail("newlocale() is not declared as locale_t newlocale(int, const char *, locale_t)");
	if (IS_T(&(newlocale), locale_t (*)(int, const char *, locale_t, int)))
		pc_fail("newlocale() is declared without a prototype");
	if (!IS_T(&(setlocale), char *(*)(int, const char *)))
		pc_fail("setlocale() is not declared as char *setlocale(int, const char *)");
	if (IS_T(&(setlocale), char *(*)(int, const char *, int)))
		pc_fail("setlocale() is declared without a prototype");
	if (!IS_T(&(uselocale), locale_t (*)(locale_t)))
		pc_fail("uselocale() is not declared as locale_t uselocale (locale_t)");
	if (IS_T(&(uselocale), locale_t (*)(locale_t, int)))
		pc_fail("uselocale() is declared without a prototype");
	pc_pass();
}
