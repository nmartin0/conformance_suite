/*
 * vsscanf() reads from a string as sscanf() does.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.vfscanf.003
 * MUTANT    vfscanf.stops_early
 * MUTANT    vfscanf.eof_zero
 */
#include "pc.h"
#include <stdarg.h>
#include <stdio.h>

static int call(const char *s, const char *fmt, ...)
{
	va_list ap;
	int r;

	va_start(ap, fmt);
	r = vsscanf(s, fmt, ap);
	va_end(ap);
	return r;
}

int main(void)
{
	int a = -1, b = -1, r;

	pc_begin("XSH.vfscanf.003");
	pc_test();
	r = call("7 8", "%d %d", &a, &b);
	pc_check(r == 2 && a == 7 && b == 8, "vsscanf(\"7 8\", \"%%d %%d\") returned %d, %d, %d", r, a, b);
	r = call("", "%d", &a);
	pc_check(r == EOF, "vsscanf(\"\", \"%%d\") returned %d, expected EOF", r);
	a = -1;
	r = call("5", "%d%d", &a, &b);
	pc_check(r == 1 && a == 5, "vsscanf(\"5\", \"%%d%%d\") returned %d and %d, expected 1 and 5", r, a);
	pc_pass();
}
