/*
 * <monetary.h> functions
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.monetary.h.004
 *
 * Only <monetary.h> is included (not pc.h, which brings in <stddef.h>), so a name
 * the header fails to provide stops the build, which the module reports as FAIL.
 */
#include <monetary.h>

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

int main(void)
{
	pc_begin("XBD.monetary.h.004");
	pc_test();
	if (!IS_T(&(strfmon), ssize_t (*)(char *, size_t, const char *, ...)))
		pc_fail("strfmon() is not declared as ssize_t strfmon(char *restrict, size_t, const char *restrict, ...)");
	if (IS_T(&(strfmon), ssize_t (*)(char *, size_t, const char *)))
		pc_fail("strfmon() is declared without a prototype");
	if (!IS_T(&(strfmon_l), ssize_t (*)(char *, size_t, locale_t, const char *, ...)))
		pc_fail("strfmon_l() is not declared as ssize_t strfmon_l(char *restrict, size_t, locale_t, const char *restrict, ...)");
	if (IS_T(&(strfmon_l), ssize_t (*)(char *, size_t, locale_t, const char *)))
		pc_fail("strfmon_l() is declared without a prototype");
	pc_pass();
}
