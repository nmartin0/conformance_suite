/*
 * Numbered conversions take their arguments from the va_list.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.vfscanf.004
 * MUTANT    vfscanf.numbered_ignored
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

	pc_begin("XSH.vfscanf.004");
	pc_test();
	r = call("10 20", "%2$d %1$d", &a, &b);
	pc_check(r == 2, "vsscanf(\"10 20\", \"%%2$d %%1$d\") returned %d, expected 2", r);
	pc_check(a == 20 && b == 10, "stored %d and %d, expected 20 in the first and 10 in the second", a, b);
	pc_pass();
}
