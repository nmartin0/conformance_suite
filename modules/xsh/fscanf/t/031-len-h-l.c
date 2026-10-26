/*
 * h and l give the size of the receiving object.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fscanf.031
 * MUTANT    fscanf.h_stores_int
 * MUTANT    fscanf.l_stores_int
 */
#include "pc.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* After pc_test(): a step that is not under test gives UNRESOLVED, not FAIL. */
#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

int main(void)
{
	short hs[4] = { 0x5a5a, 0, 0x5a5a, 0x5a5a };
	unsigned short hu[4] = { 0x5a5a, 0, 0x5a5a, 0x5a5a };
	long l = 0;
	unsigned long ul = 0;
	int r;

	pc_begin("XSH.fscanf.031");
	pc_test();
	r = sscanf("-300", "%hd", &hs[1]);
	pc_check(r == 1 && hs[1] == -300, "\"%%hd\" on \"-300\" returned %d and %d", r, hs[1]);
	pc_check(hs[0] == 0x5a5a && hs[2] == 0x5a5a, "\"%%hd\" wrote beyond the short");
	r = sscanf("65000", "%hu", &hu[1]);
	pc_check(r == 1 && hu[1] == 65000, "\"%%hu\" on \"65000\" returned %d and %u", r, (unsigned)hu[1]);
	pc_check(hu[0] == 0x5a5a && hu[2] == 0x5a5a, "\"%%hu\" wrote beyond the unsigned short");
	r = sscanf("-123456789", "%ld", &l);
	pc_check(r == 1 && l == -123456789L, "\"%%ld\" on \"-123456789\" returned %d and %ld", r, l);
	r = sscanf("4000000000", "%lu", &ul);
	pc_check(r == 1 && ul == 4000000000UL, "\"%%lu\" on \"4000000000\" returned %d and %lu", r, ul);
	pc_pass();
}
