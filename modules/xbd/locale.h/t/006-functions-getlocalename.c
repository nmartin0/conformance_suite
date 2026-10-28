/*
 * <locale.h> functions getlocalename
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.locale.h.006
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
	pc_begin("XBD.locale.h.006");
	pc_test();
	if (!IS_T(&(getlocalename_l), const char *(*)(int, locale_t)))
		pc_fail("getlocalename_l() is not declared as const char *getlocalename_l(int, locale_t)");
	if (IS_T(&(getlocalename_l), const char *(*)(int, locale_t, int)))
		pc_fail("getlocalename_l() is declared without a prototype");
	pc_pass();
}
