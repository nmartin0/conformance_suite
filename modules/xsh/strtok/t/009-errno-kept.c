/*
 * strtok() and strtok_r() leave errno alone.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strtok.009
 * MUTANT    strtok.errno_clobbered
 * MUTANT    strtok.r_errno_clobbered
 */
#include "pc.h"
#include <errno.h>
#include <string.h>

static char comma[] = ",";

int main(void)
{
	static const int before[2] = { 0, EDOM };
	char a[8], b[8], *st;
	int i, k, e;

	pc_begin("XSH.strtok.009");
	pc_test();
	for (i = 0; i < 2; i++) {
		memcpy(a, "x,y", 4);
		memcpy(b, "x,y", 4);
		for (k = 0; k < 3; k++) {
			errno = before[i];
			(void)strtok(k == 0 ? a : NULL, comma);
			e = errno;
			pc_check(e == before[i], "strtok() call %d changed errno from %s to %s",
			    k + 1, pc_errname(before[i]), pc_errname(e));
			errno = before[i];
			(void)strtok_r(k == 0 ? b : NULL, comma, &st);
			e = errno;
			pc_check(e == before[i], "strtok_r() call %d changed errno from %s to %s",
			    k + 1, pc_errname(before[i]), pc_errname(e));
		}
	}
	pc_pass();
}
