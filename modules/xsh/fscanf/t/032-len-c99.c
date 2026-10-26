/*
 * hh, ll, j, z and t give the size of the receiving object.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fscanf.032
 * MUTANT    fscanf.hh_stores_short
 * MUTANT    fscanf.j_stores_int
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
	signed char sc[4] = { 0x5a, 0, 0x5a, 0x5a };
	unsigned char uc[4] = { 0x5a, 0, 0x5a, 0x5a };
	long long ll = 0;
	intmax_t jm = 0;
	size_t z = 0;
	ptrdiff_t t = 0;
	int r;

	pc_begin("XSH.fscanf.032");
	pc_test();
	r = sscanf("-5", "%hhd", &sc[1]);
	pc_check(r == 1 && sc[1] == -5, "\"%%hhd\" on \"-5\" returned %d and %d", r, sc[1]);
	pc_check(sc[0] == 0x5a && sc[2] == 0x5a, "\"%%hhd\" wrote beyond the char");
	r = sscanf("200", "%hhu", &uc[1]);
	pc_check(r == 1 && uc[1] == 200, "\"%%hhu\" on \"200\" returned %d and %u", r, (unsigned)uc[1]);
	pc_check(uc[0] == 0x5a && uc[2] == 0x5a, "\"%%hhu\" wrote beyond the unsigned char");
	r = sscanf("-1234567890123", "%lld", &ll);
	pc_check(r == 1 && ll == -1234567890123LL, "\"%%lld\" returned %d and %lld", r, ll);
	r = sscanf("-1234567890123", "%jd", &jm);
	pc_check(r == 1 && jm == -1234567890123LL, "\"%%jd\" returned %d and %lld", r, (long long)jm);
	r = sscanf("4096", "%zu", &z);
	pc_check(r == 1 && z == 4096, "\"%%zu\" returned %d and %lu", r, (unsigned long)z);
	r = sscanf("-7", "%td", &t);
	pc_check(r == 1 && t == -7, "\"%%td\" returned %d and %ld", r, (long)t);
	pc_pass();
}
