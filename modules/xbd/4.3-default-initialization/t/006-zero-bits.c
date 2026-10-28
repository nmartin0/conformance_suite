/*
 * Where the all-zero bit pattern of a floating type is positive zero (and
 * where the implementation defines __STDC_IEC_559__ it must be), memset()
 * to zero and calloc() give the floating members of a structure the same
 * value default initialization does.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.4.3-default-initialization.006
 */
#include "pc.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

struct S { int i; float f; double d; long double ld; };

static int posz_f(float x) { return x == 0.0f && !signbit(x); }
static int posz_d(double x) { return x == 0.0 && !signbit(x); }
static int posz_l(long double x) { return x == 0.0L && !signbit(x); }

static int members_ok(const struct S *s)
{
	return s->i == 0 && posz_f(s->f) && posz_d(s->d) && posz_l(s->ld);
}

int main(void)
{
	struct S m, dflt = { 0 };
	struct S *c;
	float f;
	double d;
	long double ld;
	int zero_is_posz;

	pc_begin("XBD.4.3-default-initialization.006");
	pc_test();
	memset(&f, 0, sizeof f);
	memset(&d, 0, sizeof d);
	memset(&ld, 0, sizeof ld);
	zero_is_posz = posz_f(f) && posz_d(d) && posz_l(ld);
#ifdef __STDC_IEC_559__
	pc_check(zero_is_posz, "__STDC_IEC_559__ is defined but a float, double or long double with all bits zero is not positive zero");
#else
	if (!zero_is_posz)
		pc_notinuse("__STDC_IEC_559__ is not defined and the all-zero pattern of a floating type is not positive zero here");
#endif
	pc_check(members_ok(&dflt), "default initialization did not give positive zero members");
	memset(&m, 0xA5, sizeof m);
	memset(&m, 0, sizeof m);
	pc_check(members_ok(&m), "memset() to zero did not give the members the default value");
	c = calloc(4, sizeof *c);
	if (c == NULL)
		pc_unresolved("calloc failed");
	pc_check(members_ok(&c[0]) && members_ok(&c[3]), "calloc() did not give the members the default value");
	free(c);
	pc_pass();
}
