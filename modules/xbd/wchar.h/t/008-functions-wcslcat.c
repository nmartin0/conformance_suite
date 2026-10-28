/*
 * <wchar.h> functions wcslcat
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.wchar.h.008
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
	pc_begin("XBD.wchar.h.008");
	pc_test();
	if (!IS_T(&(wcslcat), size_t (*)(wchar_t *restrict, const wchar_t *restrict, size_t)))
		pc_fail("wcslcat() is not declared as size_t wcslcat(wchar_t *restrict, const wchar_t *restrict, size_t)");
	if (IS_T(&(wcslcat), size_t (*)(wchar_t *restrict, const wchar_t *restrict, size_t, int)))
		pc_fail("wcslcat() is declared without a prototype");
	if (!IS_T(&(wcslcpy), size_t (*)(wchar_t *restrict, const wchar_t *restrict, size_t)))
		pc_fail("wcslcpy() is not declared as size_t wcslcpy(wchar_t *restrict, const wchar_t *restrict, size_t)");
	if (IS_T(&(wcslcpy), size_t (*)(wchar_t *restrict, const wchar_t *restrict, size_t, int)))
		pc_fail("wcslcpy() is declared without a prototype");
	pc_pass();
}
