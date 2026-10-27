/*
 * Many variable arguments of mixed types.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.va_start.007
 */
#include "pc.h"
#include <stdarg.h>

static int bad;

static void reader(int n, ...)
{
	va_list ap;
	int i;

	va_start(ap, n);
	for (i = 0; i < n; i++) {
		switch (i % 3) {
		case 0:
			if (va_arg(ap, int) != i * 10) bad++;
			break;
		case 1:
			if (va_arg(ap, double) != i + 0.5) bad++;
			break;
		default:
			if (*va_arg(ap, int *) != i * 100) bad++;
			break;
		}
	}
	va_end(ap);
}

int main(void)
{
	int v2 = 200, v5 = 500, v8 = 800, v11 = 1100, v14 = 1400, v17 = 1700, v20 = 2000, v23 = 2300;

	pc_begin("XSH.va_start.007");
	pc_test();
	reader(24, 0, 1.5, &v2, 30, 4.5, &v5, 60, 7.5, &v8, 90, 10.5, &v11, 120, 13.5, &v14,
	    150, 16.5, &v17, 180, 19.5, &v20, 210, 22.5, &v23);
	pc_check(bad == 0, "%d arguments came back wrong", bad);
	pc_pass();
}
