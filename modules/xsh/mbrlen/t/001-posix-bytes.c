/*
 * In the POSIX locale mbrlen() treats every byte as a one-byte character
 * and returns 0 for the null byte.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mbrlen.001
 */
#include "pc.h"
#include <string.h>
#include <wchar.h>

int main(void)
{
	mbstate_t st;
	char buf[2];
	size_t r, rab, badr = 0;
	int b, badb = -1;

	pc_begin("XSH.mbrlen.001");
	memset(&st, 0, sizeof st);
	pc_test();
	r = mbrlen("", 1, &st);
	pc_check(r == 0, "mbrlen() of the null byte returned %zu, expected 0", r);
	for (b = 1; b <= 255 && badb < 0; b++) {
		buf[0] = (char)b;
		buf[1] = 'x';
		r = mbrlen(buf, 2, &st);
		if (r != 1) {
			badb = b;
			badr = r;
		}
	}
	pc_check(badb < 0, "mbrlen() of byte %d returned %zu, expected 1", badb, badr);
	rab = mbrlen("AB", 2, &st);
	pc_check(rab == 1, "mbrlen(\"AB\", 2) returned %zu, expected 1", rab);
	pc_pass();
}
