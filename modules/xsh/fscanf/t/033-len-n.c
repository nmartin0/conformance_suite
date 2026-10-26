/*
 * %n takes the length modifiers.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fscanf.033
 * MUTANT    fscanf.n_stores_int
 * MUTANT    fscanf.n_counted
 */
#include "pc.h"
#include <errno.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* After pc_test(): a step that is not under test gives UNRESOLVED, not FAIL. */
#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

int main(void)
{
	signed char a[8];
	short h[4];
	long l[3];
	long long ll[3];
	intmax_t j[3];
	size_t z[3];
	ptrdiff_t t[3];
	int r;

	pc_begin("XSH.fscanf.033");
	/*
	 * "%*s" stops on the space, so no conversion meets end of file
	 * before %n (where this page and ISO C disagree; see fscanf.cov).
	 */
	memset(a, 0x5a, sizeof a);
	memset(h, 0x5a, sizeof h);
	memset(l, 0x5a, sizeof l);
	memset(ll, 0x5a, sizeof ll);
	memset(j, 0x5a, sizeof j);
	memset(z, 0x5a, sizeof z);
	memset(t, 0x5a, sizeof t);
	pc_test();
	r = sscanf("abcde x", "%*s%hhn", &a[1]);
	pc_check(r == 0 && a[1] == 5, "\"%%*s%%hhn\" returned %d and %d, expected 0 and 5", r, a[1]);
	pc_check(a[0] == 0x5a && a[2] == 0x5a, "%%hhn wrote beyond the char");
	r = sscanf("abcde x", "%*s%hn", &h[1]);
	pc_check(r == 0 && h[1] == 5, "\"%%*s%%hn\" returned %d and %d, expected 0 and 5", r, h[1]);
	pc_check(h[0] == 0x5a5a && h[2] == 0x5a5a, "%%hn wrote beyond the short");
	r = sscanf("abcde x", "%*s%ln", &l[1]);
	pc_check(r == 0 && l[1] == 5, "\"%%*s%%ln\" returned %d and %ld", r, l[1]);
	r = sscanf("abcde x", "%*s%lln", &ll[1]);
	pc_check(r == 0 && ll[1] == 5, "\"%%*s%%lln\" returned %d and %lld", r, ll[1]);
	r = sscanf("abcde x", "%*s%jn", &j[1]);
	pc_check(r == 0 && j[1] == 5, "\"%%*s%%jn\" returned %d and %lld", r, (long long)j[1]);
	r = sscanf("abcde x", "%*s%zn", &z[1]);
	pc_check(r == 0 && z[1] == 5, "\"%%*s%%zn\" returned %d and %lu", r, (unsigned long)z[1]);
	r = sscanf("abcde x", "%*s%tn", &t[1]);
	pc_check(r == 0 && t[1] == 5, "\"%%*s%%tn\" returned %d and %ld", r, (long)t[1]);
	pc_check(l[0] == l[2] && ll[0] == ll[2] && j[0] == j[2] && z[0] == z[2] && t[0] == t[2],
	    "a %%n conversion wrote beyond its object");
	pc_pass();
}
