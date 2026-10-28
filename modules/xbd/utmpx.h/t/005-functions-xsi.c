/*
 * <utmpx.h> functions xsi
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.utmpx.h.005
 *
 * Only <utmpx.h> is included (not pc.h, which brings in <stddef.h>), so a name
 * the header fails to provide stops the build, which the module reports as FAIL.
 */
#include <utmpx.h>

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

int main(void)
{
	pc_begin("XBD.utmpx.h.005");
	pc_test();
	if (!IS_T(&(endutxent), void (*)(void)))
		pc_fail("endutxent() is not declared as void endutxent(void)");
	if (IS_T(&(endutxent), void (*)(int)))
		pc_fail("endutxent() is declared without a prototype");
	if (!IS_T(&(getutxent), struct utmpx *(*)(void)))
		pc_fail("getutxent() is not declared as struct utmpx *getutxent(void)");
	if (IS_T(&(getutxent), struct utmpx *(*)(int)))
		pc_fail("getutxent() is declared without a prototype");
	if (!IS_T(&(getutxid), struct utmpx *(*)(const struct utmpx *)))
		pc_fail("getutxid() is not declared as struct utmpx *getutxid(const struct utmpx *)");
	if (IS_T(&(getutxid), struct utmpx *(*)(const struct utmpx *, int)))
		pc_fail("getutxid() is declared without a prototype");
	if (!IS_T(&(getutxline), struct utmpx *(*)(const struct utmpx *)))
		pc_fail("getutxline() is not declared as struct utmpx *getutxline(const struct utmpx *)");
	if (IS_T(&(getutxline), struct utmpx *(*)(const struct utmpx *, int)))
		pc_fail("getutxline() is declared without a prototype");
	if (!IS_T(&(pututxline), struct utmpx *(*)(const struct utmpx *)))
		pc_fail("pututxline() is not declared as struct utmpx *pututxline(const struct utmpx *)");
	if (IS_T(&(pututxline), struct utmpx *(*)(const struct utmpx *, int)))
		pc_fail("pututxline() is declared without a prototype");
	if (!IS_T(&(setutxent), void (*)(void)))
		pc_fail("setutxent() is not declared as void setutxent(void)");
	if (IS_T(&(setutxent), void (*)(int)))
		pc_fail("setutxent() is declared without a prototype");
	pc_pass();
}
