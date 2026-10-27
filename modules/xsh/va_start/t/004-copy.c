/*
 * va_copy() continues from the same point.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.va_start.004
 */
#include "pc.h"
#include <stdarg.h>

static int got[16], ngot, bad;

static void reader(int n, ...)
{
	va_list ap, cp;
	int i;

	va_start(ap, n);
	(void)va_arg(ap, int);	/* 10 */
	(void)va_arg(ap, int);	/* 20 */
	va_copy(cp, ap);
	/* Advance the original past everything. */
	for (i = 2; i < n; i++)
		(void)va_arg(ap, int);
	va_end(ap);
	/* The copy still yields the remaining arguments. */
	for (i = 2; i < n; i++)
		got[ngot++] = va_arg(cp, int);
	va_end(cp);
	if (ngot != n - 2)
		bad++;
}

int main(void)
{
	int i;

	pc_begin("XSH.va_start.004");
	pc_test();
	reader(6, 10, 20, 30, 40, 50, 60);
	pc_check(ngot == 4, "the copy yielded %d arguments, expected 4", ngot);
	for (i = 0; i < 4; i++)
		pc_check(got[i] == 30 + 10 * i, "copy argument %d is %d, expected %d", i, got[i], 30 + 10 * i);
	pc_check(bad == 0, "the count was wrong");
	pc_pass();
}
