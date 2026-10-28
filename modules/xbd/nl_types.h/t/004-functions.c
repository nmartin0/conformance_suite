/*
 * <nl_types.h> functions
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.nl_types.h.004
 *
 * Only <nl_types.h> is included (not pc.h, which brings in <stddef.h>), so a name
 * the header fails to provide stops the build, which the module reports as FAIL.
 */
#include <nl_types.h>

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

int main(void)
{
	pc_begin("XBD.nl_types.h.004");
	pc_test();
	if (!IS_T(&(catclose), int (*)(nl_catd)))
		pc_fail("catclose() is not declared as int catclose(nl_catd)");
	if (IS_T(&(catclose), int (*)(nl_catd, int)))
		pc_fail("catclose() is declared without a prototype");
	if (!IS_T(&(catgets), char *(*)(nl_catd, int, int, const char *)))
		pc_fail("catgets() is not declared as char *catgets(nl_catd, int, int, const char *)");
	if (IS_T(&(catgets), char *(*)(nl_catd, int, int, const char *, int)))
		pc_fail("catgets() is declared without a prototype");
	if (!IS_T(&(catopen), nl_catd (*)(const char *, int)))
		pc_fail("catopen() is not declared as nl_catd catopen(const char *, int)");
	if (IS_T(&(catopen), nl_catd (*)(const char *, int, int)))
		pc_fail("catopen() is declared without a prototype");
	pc_pass();
}
