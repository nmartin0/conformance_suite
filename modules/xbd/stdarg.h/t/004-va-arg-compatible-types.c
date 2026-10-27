/*
 * va_arg with the permitted type mismatches.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.stdarg.h.004
 */
#include <stdarg.h>
#include "pc.h"

static int a;
static unsigned b;
static long c;
static unsigned long d;
static long long e;
static unsigned long long f;
static char *p1;
static void *p2, *p3;
static unsigned char *p4;

static void h(int n, ...)
{
	va_list ap;

	va_start(ap, n);
	a = va_arg(ap, int);
	b = va_arg(ap, unsigned);
	c = va_arg(ap, long);
	d = va_arg(ap, unsigned long);
	e = va_arg(ap, long long);
	f = va_arg(ap, unsigned long long);
	p1 = va_arg(ap, char *);
	p2 = va_arg(ap, void *);
	p3 = va_arg(ap, void *);
	p4 = va_arg(ap, unsigned char *);
	va_end(ap);
}

int main(void)
{
	pc_begin("XBD.stdarg.h.004");
	pc_test();
	static char ch[2];

	h(0, 7u, 8, 9ul, 10L, 11ull, 12LL, (void *)ch, (char *)ch + 1, (unsigned char *)ch, (void *)(ch + 1));
	pc_check(a == 7, "an unsigned argument read as int gave %d", a);
	pc_check(b == 8, "an int argument read as unsigned gave %u", b);
	pc_check(c == 9, "an unsigned long argument read as long gave %ld", c);
	pc_check(d == 10, "a long argument read as unsigned long gave %lu", d);
	pc_check(e == 11, "an unsigned long long argument read as long long gave %lld", e);
	pc_check(f == 12, "a long long argument read as unsigned long long gave %llu", f);
	pc_check(p1 == ch, "a void * argument read as char * gave a different pointer");
	pc_check(p2 == (void *)(ch + 1), "a char * argument read as void * gave a different pointer");
	pc_check(p3 == (void *)ch, "an unsigned char * argument read as void * gave a different pointer");
	pc_check(p4 == (unsigned char *)(ch + 1), "a void * argument read as unsigned char * gave a different pointer");
	pc_pass();
}
