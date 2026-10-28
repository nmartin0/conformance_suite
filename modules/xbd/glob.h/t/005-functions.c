/*
 * <glob.h> functions
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.glob.h.005
 *
 * Only <glob.h> is included (not pc.h, which brings in <stddef.h>), so a name
 * the header fails to provide stops the build, which the module reports as FAIL.
 */
#include <glob.h>

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

int main(void)
{
	pc_begin("XBD.glob.h.005");
	pc_test();
	if (!IS_T(&(glob), int (*)(const char *restrict, int, int(*)(const char *, int), glob_t *restrict)))
		pc_fail("glob() is not declared as int glob(const char *restrict, int, int(*)(const char *, int), glob_t *restrict)");
	if (IS_T(&(glob), int (*)(const char *restrict, int, int(*)(const char *, int), glob_t *restrict, int)))
		pc_fail("glob() is declared without a prototype");
	if (!IS_T(&(globfree), void (*)(glob_t *)))
		pc_fail("globfree() is not declared as void globfree(glob_t *)");
	if (IS_T(&(globfree), void (*)(glob_t *, int)))
		pc_fail("globfree() is declared without a prototype");
	pc_pass();
}
