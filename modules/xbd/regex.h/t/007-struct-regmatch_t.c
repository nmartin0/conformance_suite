/*
 * <regex.h> struct regmatch_t
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.regex.h.007
 *
 * Only <regex.h> is included (not pc.h, which brings in <stddef.h>), so a name
 * the header fails to provide stops the build, which the module reports as FAIL.
 */
#include <regex.h>

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

typedef char pc_c0[sizeof(regmatch_t)];	/* a complete type */
static regmatch_t *ps;

int main(void)
{
	pc_begin("XBD.regex.h.007");
	pc_test();
	(void)ps;
	if (!IS_T(&ps->rm_so, regoff_t *))
		pc_fail("regmatch_t member rm_so is not of type regoff_t");
	if (!IS_T(&ps->rm_eo, regoff_t *))
		pc_fail("regmatch_t member rm_eo is not of type regoff_t");
	pc_pass();
}
