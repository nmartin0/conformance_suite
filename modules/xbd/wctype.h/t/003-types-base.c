/*
 * <wctype.h> types base
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.wctype.h.003
 *
 * Only <wctype.h> is included (not pc.h, which brings in <stddef.h>), so a name
 * the header fails to provide stops the build, which the module reports as FAIL.
 */
#include <wctype.h>

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

typedef wint_t *pc_t0;
typedef wctrans_t *pc_t1;
typedef wctype_t *pc_t2;

int main(void)
{
	pc_begin("XBD.wctype.h.003");
	pc_test();
	pc_pass();
}
