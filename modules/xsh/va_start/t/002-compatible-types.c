/*
 * va_arg() with the type pairs the header allows.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.va_start.002
 */
#include "pc.h"
#include <stdarg.h>

static int bad;

static void reader(int n, ...)
{
	va_list ap;

	va_start(ap, n);
	/* passed as int/long/long long, read as the unsigned type */
	if (va_arg(ap, unsigned int) != 77u) bad++;
	if (va_arg(ap, unsigned long) != 123456UL) bad++;
	if (va_arg(ap, unsigned long long) != 98765432123ULL) bad++;
	/* passed as unsigned, read as the signed type */
	if (va_arg(ap, int) != 55) bad++;
	if (va_arg(ap, long) != 654321L) bad++;
	if (va_arg(ap, long long) != 12345678901LL) bad++;
	/* void * read as char *, char * read as void * */
	{
		char *cp = va_arg(ap, char *);
		void *vp = va_arg(ap, void *);

		if (cp == NULL || *cp != 'q') bad++;
		if (vp == NULL || *(char *)vp != 'r') bad++;
	}
	(void)n;
	va_end(ap);
}

int main(void)
{
	char q = 'q', r = 'r';
	void *vq = &q;
	char *cr = &r;

	pc_begin("XSH.va_start.002");
	pc_test();
	reader(0, 77, 123456L, 98765432123LL, 55u, 654321UL, 12345678901ULL, vq, cr);
	pc_check(bad == 0, "%d arguments came back wrong", bad);
	pc_pass();
}
