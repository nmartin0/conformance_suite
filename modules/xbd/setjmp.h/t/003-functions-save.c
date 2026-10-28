/*
 * <setjmp.h> functions save
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.setjmp.h.003
 *
 * Only <setjmp.h> is included (not pc.h, which brings in <stddef.h>), so a name
 * the header fails to provide stops the build, which the module reports as FAIL.
 */
#include <setjmp.h>

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

int main(void)
{
	pc_begin("XBD.setjmp.h.003");
	pc_test();
#ifndef setjmp
	if (!IS_T(&(setjmp), int (*)(jmp_buf)))
		pc_fail("setjmp() is not declared as int setjmp(jmp_buf)");
	if (IS_T(&(setjmp), int (*)(jmp_buf, int)))
		pc_fail("setjmp() is declared without a prototype");
#endif
#ifndef sigsetjmp
	if (!IS_T(&(sigsetjmp), int (*)(sigjmp_buf, int)))
		pc_fail("sigsetjmp() is not declared as int sigsetjmp(sigjmp_buf, int)");
	if (IS_T(&(sigsetjmp), int (*)(sigjmp_buf, int, int)))
		pc_fail("sigsetjmp() is declared without a prototype");
#endif
	pc_pass();
}
