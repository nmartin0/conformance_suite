/*
 * <ftw.h> struct ftw
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.ftw.h.003
 *
 * Only <ftw.h> is included (not pc.h, which brings in <stddef.h>), so a name
 * the header fails to provide stops the build, which the module reports as FAIL.
 */
#include <ftw.h>

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

typedef char pc_c0[sizeof(struct FTW)];	/* a complete type */
static struct FTW *ps;

int main(void)
{
	pc_begin("XBD.ftw.h.003");
	pc_test();
	(void)ps;
	if (!IS_T(&ps->base, int *))
		pc_fail("struct FTW member base is not of type int");
	if (!IS_T(&ps->level, int *))
		pc_fail("struct FTW member level is not of type int");
	pc_pass();
}
