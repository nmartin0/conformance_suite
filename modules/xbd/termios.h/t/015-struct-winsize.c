/*
 * <termios.h> struct winsize
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.termios.h.015
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

typedef char pc_c0[sizeof(struct winsize)];	/* a complete type */
static struct winsize *ps;

int main(void)
{
	pc_begin("XBD.termios.h.015");
	pc_test();
	(void)ps;
	if (!IS_T(&ps->ws_row, unsigned short *))
		pc_fail("struct winsize member ws_row is not of type unsigned short");
	if (!IS_T(&ps->ws_col, unsigned short *))
		pc_fail("struct winsize member ws_col is not of type unsigned short");
	pc_pass();
}
