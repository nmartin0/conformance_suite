/*
 * <sys/select.h> functions fd macros
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sys_select.h.007
 *
 * Only <sys/select.h> is included (not pc.h, which brings in <stddef.h>), so a name
 * the header fails to provide stops the build, which the module reports as FAIL.
 */
#include <sys/select.h>

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

int main(void)
{
	pc_begin("XBD.sys_select.h.007");
	pc_test();
#ifndef FD_CLR
	if (!IS_T(&(FD_CLR), void (*)(int, fd_set *)))
		pc_fail("FD_CLR() is not declared as void FD_CLR(int, fd_set *)");
	if (IS_T(&(FD_CLR), void (*)(int, fd_set *, int)))
		pc_fail("FD_CLR() is declared without a prototype");
#endif
#ifndef FD_ISSET
	if (!IS_T(&(FD_ISSET), int (*)(int, const fd_set *)))
		pc_fail("FD_ISSET() is not declared as int FD_ISSET(int, const fd_set *)");
	if (IS_T(&(FD_ISSET), int (*)(int, const fd_set *, int)))
		pc_fail("FD_ISSET() is declared without a prototype");
#endif
#ifndef FD_SET
	if (!IS_T(&(FD_SET), void (*)(int, fd_set *)))
		pc_fail("FD_SET() is not declared as void FD_SET(int, fd_set *)");
	if (IS_T(&(FD_SET), void (*)(int, fd_set *, int)))
		pc_fail("FD_SET() is declared without a prototype");
#endif
#ifndef FD_ZERO
	if (!IS_T(&(FD_ZERO), void (*)(fd_set *)))
		pc_fail("FD_ZERO() is not declared as void FD_ZERO(fd_set *)");
	if (IS_T(&(FD_ZERO), void (*)(fd_set *, int)))
		pc_fail("FD_ZERO() is declared without a prototype");
#endif
	pc_pass();
}
