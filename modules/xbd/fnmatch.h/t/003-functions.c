/*
 * <fnmatch.h> functions
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.fnmatch.h.003
 *
 * Only <fnmatch.h> is included (not pc.h, which brings in <stddef.h>), so a name
 * the header fails to provide stops the build, which the module reports as FAIL.
 */
#include <fnmatch.h>

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

int main(void)
{
	pc_begin("XBD.fnmatch.h.003");
	pc_test();
	if (!IS_T(&(fnmatch), int (*)(const char *, const char *, int)))
		pc_fail("fnmatch() is not declared as int fnmatch(const char *, const char *, int)");
	if (IS_T(&(fnmatch), int (*)(const char *, const char *, int, int)))
		pc_fail("fnmatch() is declared without a prototype");
	pc_pass();
}
