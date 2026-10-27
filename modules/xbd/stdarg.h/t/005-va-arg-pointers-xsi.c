/*
 * va_arg between different pointer types (XSI).
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.stdarg.h.005
 */
#include <stdarg.h>
#include "pc.h"

struct st { int x; };

static long *q1;
static double *q2;
static struct st *q3;

static void h(int n, ...)
{
	va_list ap;

	va_start(ap, n);
	q1 = va_arg(ap, long *);
	q2 = va_arg(ap, double *);
	q3 = va_arg(ap, struct st *);
	va_end(ap);
}

int main(void)
{
	pc_begin("XBD.stdarg.h.005");
	pc_test();
	static int x;
	static struct st s;
	static float f;

	h(0, &x, &s, &f);
	pc_check((void *)q1 == (void *)&x, "an int * read as long * did not come back unchanged");
	pc_check((void *)q2 == (void *)&s, "a struct pointer read as double * did not come back unchanged");
	pc_check((void *)q3 == (void *)&f, "a float * read as a struct pointer did not come back unchanged");
	pc_pass();
}
