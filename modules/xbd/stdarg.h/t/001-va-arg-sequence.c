/*
 * va_start and va_arg return the arguments in order.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.stdarg.h.001
 */
#include <stdarg.h>
#include <string.h>
#include "pc.h"

struct big { int a[5]; };

static int g_i, g_last;
static double g_d;
static long g_l;
static long long g_ll;
static const char *g_s;
static void *g_p;
static struct big g_b;

static void mix(int n, ...)
{
	va_list ap;

	va_start(ap, n);
	g_i = va_arg(ap, int);
	g_d = va_arg(ap, double);
	g_l = va_arg(ap, long);
	g_ll = va_arg(ap, long long);
	g_s = va_arg(ap, const char *);
	g_p = va_arg(ap, void *);
	g_b = va_arg(ap, struct big);
	g_last = va_arg(ap, int);
	va_end(ap);
}

int main(void)
{
	pc_begin("XBD.stdarg.h.001");
	pc_test();
	static int target;
	struct big b = { { 1, 2, 3, 4, 5 } };

	mix(0, -17, 2.5, -123456789L, 1234567890123LL, "text", (void *)&target, b, 99);
	pc_check(g_i == -17, "the int argument was read as %d", g_i);
	pc_check(g_d == 2.5, "the double argument was read as %g", g_d);
	pc_check(g_l == -123456789L, "the long argument was read as %ld", g_l);
	pc_check(g_ll == 1234567890123LL, "the long long argument was read as %lld", g_ll);
	pc_check(strcmp(g_s, "text") == 0, "the string argument was read wrongly");
	pc_check(g_p == (void *)&target, "the pointer argument was read wrongly");
	pc_check(memcmp(&g_b, &b, sizeof b) == 0, "the structure argument was read wrongly");
	pc_check(g_last == 99, "the argument after the structure was read as %d", g_last);
	pc_pass();
}
