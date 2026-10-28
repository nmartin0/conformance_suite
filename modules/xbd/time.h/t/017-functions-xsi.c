/*
 * <time.h> functions xsi
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.time.h.017
 *
 * Only <time.h> is included (not pc.h, which brings in <stddef.h>), so a name
 * the header fails to provide stops the build, which the module reports as FAIL.
 */
#include <time.h>

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

int main(void)
{
	pc_begin("XBD.time.h.017");
	pc_test();
	if (!IS_T(&(getdate), struct tm *(*)(const char *)))
		pc_fail("getdate() is not declared as struct tm *getdate(const char *)");
	if (IS_T(&(getdate), struct tm *(*)(const char *, int)))
		pc_fail("getdate() is declared without a prototype");
	if (!IS_T(&(strptime), char *(*)(const char *restrict, const char *restrict, struct tm *restrict)))
		pc_fail("strptime() is not declared as char *strptime(const char *restrict, const char *restrict, struct tm *restrict)");
	if (IS_T(&(strptime), char *(*)(const char *restrict, const char *restrict, struct tm *restrict, int)))
		pc_fail("strptime() is declared without a prototype");
	pc_pass();
}
