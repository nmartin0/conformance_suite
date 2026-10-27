/*
 * va_arg() returns the arguments in order.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.va_start.001
 */
#include "pc.h"
#include <stdarg.h>
#include <string.h>

static int bad;
static int seen;

/* Reads: int, long, long long, double, long double, char *, then char and short (promoted), float (promoted). */
static void reader(const char *fmt, ...)
{
	va_list ap;
	const char *p;

	va_start(ap, fmt);
	for (p = fmt; *p; p++) {
		seen++;
		switch (*p) {
		case 'i': if (va_arg(ap, int) != -12345) bad++; break;
		case 'l': if (va_arg(ap, long) != 1234567890L) bad++; break;
		case 'q': if (va_arg(ap, long long) != -1234567890123LL) bad++; break;
		case 'd': if (va_arg(ap, double) != 2.5) bad++; break;
		case 'D': if (va_arg(ap, long double) != 3.25L) bad++; break;
		case 's': if (strcmp(va_arg(ap, char *), "text") != 0) bad++; break;
		case 'c': if (va_arg(ap, int) != 'Z') bad++; break;
		case 'h': if (va_arg(ap, int) != -300) bad++; break;
		case 'f': if (va_arg(ap, double) != 1.5) bad++; break;
		default: bad++; break;
		}
	}
	va_end(ap);
}

static int count_none(const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	va_end(ap);
	return fmt != NULL;
}

int main(void)
{
	char c = 'Z';
	short h = -300;
	float f = 1.5f;

	pc_begin("XSH.va_start.001");
	pc_test();
	reader("ilqdDschf", -12345, 1234567890L, -1234567890123LL, 2.5, 3.25L, "text", c, h, f);
	pc_check(seen == 9, "reader read %d arguments, expected 9", seen);
	pc_check(bad == 0, "%d arguments came back wrong", bad);
	seen = 0;
	reader("dsi", 2.5, "text", -12345);
	pc_check(seen == 3 && bad == 0, "second call: seen %d, bad %d", seen, bad);
	pc_check(count_none("x") == 1, "a function with no variable arguments did not work");
	pc_pass();
}
