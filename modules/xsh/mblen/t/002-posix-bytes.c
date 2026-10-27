/*
 * In the POSIX locale every byte is a one-byte character; the null byte
 * gives 0.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mblen.002
 */
#include "pc.h"
#include <errno.h>
#include <stdlib.h>

int main(void)
{
	char buf[3];
	int b, r, badb = -1, badr = 0, badn = 0, n;

	pc_begin("XSH.mblen.002");
	pc_test();
	buf[0] = '\0';
	buf[1] = 'x';
	r = mblen(buf, 1);
	pc_check(r == 0, "mblen() of the null byte returned %d, expected 0", r);
	for (b = 1; b <= 255 && badb < 0; b++)
		for (n = 1; n <= 3 && badb < 0; n += 2) {
			buf[0] = (char)b;
			buf[1] = buf[2] = 'x';
			r = mblen(buf, (size_t)n);
			if (r != 1) {
				badb = b;
				badr = r;
				badn = n;
			}
		}
	pc_check(badb < 0, "mblen() of byte %d with n %d returned %d, expected 1", badb, badn, badr);
	pc_pass();
}
