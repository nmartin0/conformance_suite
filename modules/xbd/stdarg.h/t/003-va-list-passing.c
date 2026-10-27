/*
 * Passing a va_list to a callee and traversing twice.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.stdarg.h.003
 */
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "pc.h"

static int sum(int n, va_list ap)
{
	int s = 0;

	while (n-- > 0)
		s += va_arg(ap, int);
	return s;
}

static int twice, first, second, fmt_len;
static char text[64];

static void f(int n, ...)
{
	va_list ap;

	va_start(ap, n);
	first = sum(n, ap);
	va_end(ap);
	va_start(ap, n);
	second = sum(n, ap);
	va_end(ap);
	twice = first + second;
}

static void g(const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	fmt_len = vsnprintf(text, sizeof text, fmt, ap);
	va_end(ap);
}

int main(void)
{
	pc_begin("XBD.stdarg.h.003");
	pc_test();
	f(4, 1, 2, 3, 4);
	pc_check(first == 10 && second == 10, "two traversals gave %d and %d, expected 10 and 10", first, second);
	g("%d-%s-%d", 5, "x", 7);
	pc_check(fmt_len == 5 && strcmp(text, "5-x-7") == 0, "vsnprintf with a passed va_list gave \"%s\"", text);
	pc_pass();
}
