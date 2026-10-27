/*
 * va_copy gives an independent list in the same state.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.stdarg.h.002
 */
#include <stdarg.h>
#include "pc.h"

static int r1[3], r2[3], skipped;

static void f(int n, ...)
{
	va_list ap, cp;
	int i, a1, a2;

	va_start(ap, n);
	a1 = va_arg(ap, int);
	a2 = va_arg(ap, int);
	skipped = a1 + a2;
	va_copy(cp, ap);
	for (i = 0; i < 3; i++)
		r2[i] = va_arg(cp, int);
	va_end(cp);
	for (i = 0; i < 3; i++)
		r1[i] = va_arg(ap, int);
	va_end(ap);
}

int main(void)
{
	pc_begin("XBD.stdarg.h.002");
	pc_test();
	int i;

	f(0, 10, 20, 31, 32, 33);
	pc_check(skipped == 30, "the first two arguments were read as %d", skipped);
	for (i = 0; i < 3; i++) {
		pc_check(r2[i] == 31 + i, "the copy gave %d for argument %d", r2[i], i + 3);
		pc_check(r1[i] == 31 + i, "the original, after the copy was read, gave %d for argument %d", r1[i], i + 3);
	}
	pc_pass();
}
