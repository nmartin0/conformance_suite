/*
 * <termios.h> functions winsize
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.termios.h.017
 *
 * Only <termios.h> is included (not pc.h, which brings in <stddef.h>), so a name
 * the header fails to provide stops the build, which the module reports as FAIL.
 */
#include <termios.h>

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

int main(void)
{
	pc_begin("XBD.termios.h.017");
	pc_test();
	if (!IS_T(&(tcgetwinsize), int (*)(int, struct winsize *)))
		pc_fail("tcgetwinsize() is not declared as int tcgetwinsize(int, struct winsize *)");
	if (IS_T(&(tcgetwinsize), int (*)(int, struct winsize *, int)))
		pc_fail("tcgetwinsize() is declared without a prototype");
	if (!IS_T(&(tcsetwinsize), int (*)(int, const struct winsize *)))
		pc_fail("tcsetwinsize() is not declared as int tcsetwinsize(int, const struct winsize *)");
	if (IS_T(&(tcsetwinsize), int (*)(int, const struct winsize *, int)))
		pc_fail("tcsetwinsize() is declared without a prototype");
	pc_pass();
}
