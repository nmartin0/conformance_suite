/*
 * On XSI systems, va_arg() may read one pointer type as another.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.va_start.003
 */
#include "pc.h"
#include <stdarg.h>

static int ga = 1, gb = 2;
static char gc = 3;
static int bad;

static void reader(int n, ...)
{
	va_list ap;
	long *lp;
	double *dp;
	int *ip;

	(void)n;
	va_start(ap, n);
	lp = va_arg(ap, long *);	/* passed as int * */
	dp = va_arg(ap, double *);	/* passed as int * */
	ip = va_arg(ap, int *);	/* passed as char * */
	va_end(ap);
	if ((void *)lp != (void *)&ga)
		bad++;
	if ((void *)dp != (void *)&gb)
		bad++;
	if ((void *)ip != (void *)&gc)
		bad++;
}

int main(void)
{
	pc_begin("XSH.va_start.003");
	pc_test();
	reader(0, &ga, &gb, &gc);
	pc_check(bad == 0, "%d pointers came back different when read as another pointer type", bad);
	pc_pass();
}
