/*
 * <monetary.h> types ssize
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.monetary.h.003
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

typedef ssize_t *pc_t0;

int main(void)
{
	pc_begin("XBD.monetary.h.003");
	pc_test();
	if ((ssize_t)-1 >= 0)
		pc_fail("ssize_t is not a signed integer type");
	pc_pass();
}
