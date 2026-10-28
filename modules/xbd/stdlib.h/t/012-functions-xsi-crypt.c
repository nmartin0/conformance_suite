/*
 * <stdlib.h> functions xsi crypt
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.stdlib.h.012
 *
 * Only <stdlib.h> is included (not pc.h, which brings in <stddef.h>), so a name
 * the header fails to provide stops the build, which the module reports as FAIL.
 */
#include <stdlib.h>

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

int main(void)
{
	pc_begin("XBD.stdlib.h.012");
	pc_test();
	if (!IS_T(&(setkey), void (*)(const char *)))
		pc_fail("setkey() is not declared as void setkey(const char *)");
	if (IS_T(&(setkey), void (*)(const char *, int)))
		pc_fail("setkey() is declared without a prototype");
	pc_pass();
}
