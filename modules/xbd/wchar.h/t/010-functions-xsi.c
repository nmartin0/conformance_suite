/*
 * <wchar.h> functions xsi
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.wchar.h.010
 *
 * Only <wchar.h> is included (not pc.h, which brings in <stddef.h>), so a name
 * the header fails to provide stops the build, which the module reports as FAIL.
 */
#include <wchar.h>

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

int main(void)
{
	pc_begin("XBD.wchar.h.010");
	pc_test();
	if (!IS_T(&(wcswidth), int (*)(const wchar_t *, size_t)))
		pc_fail("wcswidth() is not declared as int wcswidth(const wchar_t *, size_t)");
	if (IS_T(&(wcswidth), int (*)(const wchar_t *, size_t, int)))
		pc_fail("wcswidth() is declared without a prototype");
	if (!IS_T(&(wcwidth), int (*)(wchar_t)))
		pc_fail("wcwidth() is not declared as int wcwidth(wchar_t)");
	if (IS_T(&(wcwidth), int (*)(wchar_t, int)))
		pc_fail("wcwidth() is declared without a prototype");
	pc_pass();
}
