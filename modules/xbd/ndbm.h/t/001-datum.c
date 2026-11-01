/*
 * <ndbm.h> datum
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.ndbm.h.001
 *
 * Only <ndbm.h> is included (not pc.h, which brings in other headers), so a
 * name the header fails to provide stops the build, which the module
 * reports as FAIL.
 */
#include <ndbm.h>

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

typedef char pc_c0[sizeof(datum)];	/* a complete type */
static datum *ps;

int main(void)
{
	pc_begin("XBD.ndbm.h.001");
	pc_test();
	(void)ps;
	if (!IS_T(&ps->dptr, void * *))
		pc_fail("datum member dptr is not of type void *");
	if (!IS_T(&ps->dsize, size_t *))
		pc_fail("datum member dsize is not of type size_t");
	pc_pass();
}
