/*
 * <time.h> variables tzname
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.time.h.013
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
	pc_begin("XBD.time.h.013");
	pc_test();
	if (!IS_T(&tzname[0], char * *))
		pc_fail("tzname is not an array of char *");
	if (IS_T(&tzname, char * * *))
		pc_fail("tzname is not an array");
	pc_pass();
}
