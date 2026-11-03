/*
 * <strings.h> functions
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.strings.h.003
 *
 * Only <strings.h> is included (not pc.h, which brings in <stddef.h>), so a name
 * the header fails to provide stops the build, which the module reports as FAIL.
 */
#include <strings.h>

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

int main(void)
{
	pc_begin("XBD.strings.h.003");
	pc_test();
	if (!IS_T(&(strcasecmp), int (*)(const char *, const char *)))
		pc_fail("strcasecmp() is not declared as int strcasecmp(const char *, const char *)");
	if (IS_T(&(strcasecmp), int (*)(const char *, const char *, int)))
		pc_fail("strcasecmp() is declared without a prototype");
	if (!IS_T(&(strcasecmp_l), int (*)(const char *, const char *, locale_t)))
		pc_fail("strcasecmp_l() is not declared as int strcasecmp_l(const char *, const char *, locale_t)");
	if (IS_T(&(strcasecmp_l), int (*)(const char *, const char *, locale_t, int)))
		pc_fail("strcasecmp_l() is declared without a prototype");
	if (!IS_T(&(strncasecmp), int (*)(const char *, const char *, size_t)))
		pc_fail("strncasecmp() is not declared as int strncasecmp(const char *, const char *, size_t)");
	if (IS_T(&(strncasecmp), int (*)(const char *, const char *, size_t, int)))
		pc_fail("strncasecmp() is declared without a prototype");
	if (!IS_T(&(strncasecmp_l), int (*)(const char *, const char *, size_t, locale_t)))
		pc_fail("strncasecmp_l() is not declared as int strncasecmp_l(const char *, const char *, size_t, locale_t)");
	if (IS_T(&(strncasecmp_l), int (*)(const char *, const char *, size_t, locale_t, int)))
		pc_fail("strncasecmp_l() is declared without a prototype");
	pc_pass();
}
