/*
 * off_t, pid_t, ssize_t, ptrdiff_t and suseconds_t are signed integer types.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.11-data-types.001
 */
#include "pc.h"
#include <stddef.h>
#include <sys/select.h>
#include <sys/types.h>

/* Bitwise and shift operators are accepted only for integer types, so a build failure is a failure. */
#define OPS(T) ((T)((((T)1 << 1) | ((T)6 & (T)3)) ^ ~(T)0) >> 1)
#define SIGNED(T) (!((T)~(T)0 > (T)0))

_Static_assert(SIGNED(off_t), "off_t is not signed");
_Static_assert(SIGNED(pid_t), "pid_t is not signed");
_Static_assert(SIGNED(ssize_t), "ssize_t is not signed");
_Static_assert(SIGNED(ptrdiff_t), "ptrdiff_t is not signed");
_Static_assert(SIGNED(suseconds_t), "suseconds_t is not signed");

int main(void)
{
	volatile int v;

	pc_begin("XSH.2.11-data-types.001");
	pc_test();
	v = (OPS(off_t) != 0) + (OPS(pid_t) != 0) + (OPS(ssize_t) != 0) + (OPS(ptrdiff_t) != 0) + (OPS(suseconds_t) != 0);
	(void)v;
	pc_check((off_t)-1 < 0, "off_t is not signed");
	pc_check((pid_t)-1 < 0, "pid_t is not signed");
	pc_check((ssize_t)-1 < 0, "ssize_t is not signed");
	pc_check((ptrdiff_t)-1 < 0, "ptrdiff_t is not signed");
	pc_check((suseconds_t)-1 < 0, "suseconds_t is not signed");
	pc_pass();
}
