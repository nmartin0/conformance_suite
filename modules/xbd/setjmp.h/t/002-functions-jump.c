/*
 * <setjmp.h> functions jump
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.setjmp.h.002
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
	pc_begin("XBD.setjmp.h.002");
	pc_test();
	if (!IS_T(&(longjmp), void (*)(jmp_buf, int)))
		pc_fail("longjmp() is not declared as _Noreturn void longjmp(jmp_buf, int)");
	if (IS_T(&(longjmp), void (*)(jmp_buf, int, int)))
		pc_fail("longjmp() is declared without a prototype");
	if (!IS_T(&(siglongjmp), void (*)(sigjmp_buf, int)))
		pc_fail("siglongjmp() is not declared as _Noreturn void siglongjmp(sigjmp_buf, int)");
	if (IS_T(&(siglongjmp), void (*)(sigjmp_buf, int, int)))
		pc_fail("siglongjmp() is declared without a prototype");
	pc_pass();
}
