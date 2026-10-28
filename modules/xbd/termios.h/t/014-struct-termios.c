/*
 * <termios.h> struct termios
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.termios.h.014
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

typedef char pc_c0[sizeof(struct termios)];	/* a complete type */
static struct termios *ps;

int main(void)
{
	pc_begin("XBD.termios.h.014");
	pc_test();
	(void)ps;
	if (!IS_T(&ps->c_iflag, tcflag_t *))
		pc_fail("struct termios member c_iflag is not of type tcflag_t");
	if (!IS_T(&ps->c_oflag, tcflag_t *))
		pc_fail("struct termios member c_oflag is not of type tcflag_t");
	if (!IS_T(&ps->c_cflag, tcflag_t *))
		pc_fail("struct termios member c_cflag is not of type tcflag_t");
	if (!IS_T(&ps->c_lflag, tcflag_t *))
		pc_fail("struct termios member c_lflag is not of type tcflag_t");
	if (!IS_T(&ps->c_cc[0], cc_t *))
		pc_fail("struct termios member c_cc is not an array of cc_t");
	if (!IS_T(&ps->c_cc, cc_t (*)[NCCS]))
		pc_fail("struct termios member c_cc is not an array");
	pc_pass();
}
