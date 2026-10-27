/*
 * Right precision and rounding.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strfmon.003
 */
#include "fmon.h"

static void shape(const char *fmt, double v)
{
	char buf[128];
	size_t i;

	fm_ok(buf, sizeof buf, fmt, v);
	pc_check(strpbrk(buf, ".,") == NULL, "strfmon(\"%s\", %g) gave \"%s\", which has a radix or separator character", fmt, v, buf);
	for (i = 0; buf[i] != '\0'; i++)
		pc_check((buf[i] >= '0' && buf[i] <= '9') || buf[i] == ' ', "strfmon(\"%s\", %g) gave \"%s\", with the byte 0x%02x that is neither a digit nor a space", fmt, v, buf, (unsigned)(unsigned char)buf[i]);
}

int main(void)
{
	pc_begin("XSH.strfmon.003");
	lc_init();
	if (setlocale(LC_ALL, "C") == NULL)
		pc_unresolved("setlocale(LC_ALL, \"C\") failed");
	pc_test();
	fm_want("%.0n", 123.0, "123");
	fm_want("%.0n", 123.6, "124");
	fm_want("%.0n", 123.4, "123");
	shape("%.0n", 123.0);
	shape("%.0n", 123.6);
	fm_want("%.2n", 123.456, "12346");
	fm_want("%.4i", 1.5, "15000");
	fm_want("%.0i", 123.6, "124");
	shape("%.0i", 123.6);
	pc_pass();
}
