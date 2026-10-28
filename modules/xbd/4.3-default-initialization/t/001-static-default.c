/*
 * Objects with static (or thread) storage duration and no initializer
 * start as default initialization says: null pointers, positive zero,
 * every aggregate member likewise, and the first named member of a union.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.4.3-default-initialization.001
 */
#include "pc.h"
#include <math.h>
#include <stddef.h>
#include <stdlib.h>

struct inner { float f; void *q; int arr[3]; };
struct outer {
	int *p;
	double d;
	long double ld;
	struct inner in;
	struct inner ins[2];
	char s[5];
	unsigned char uc;
	_Bool b;
	enum { E0, E1 } e;
};
union U { void *u; double dd; char c[16]; };
union V { struct inner in; long l; };
struct hasU { int tag; union U un; union V vn; };
union W { double first; void *second; };

static struct outer g_o;
static union U g_u;
static union V g_v;
static union W g_w;
static struct hasU g_hu[2];
static int *g_pa[3];
static double g_da[4];
static float g_fa;
static long double g_lda;
static char g_str[20];
static void *g_vp;
static int (*g_fp)(void);
static unsigned char g_uc;
static short g_sh;
static long long g_ll;
static double _Complex g_z;
static _Thread_local int tl_i;
static _Thread_local struct outer tl_o;
int ext_i;
struct outer ext_o;

static int posz_d(double x) { return x == 0.0 && !signbit(x); }
static int posz_f(float x) { return x == 0.0f && !signbit(x); }
static int posz_l(long double x) { return x == 0.0L && !signbit(x); }

static int inner_ok(const struct inner *p)
{
	int i;

	if (!posz_f(p->f) || p->q != NULL)
		return 0;
	for (i = 0; i < 3; i++)
		if (p->arr[i] != 0)
			return 0;
	return 1;
}

static int outer_ok(const struct outer *o)
{
	int i;

	if (o->p != NULL || !posz_d(o->d) || !posz_l(o->ld) || !inner_ok(&o->in))
		return 0;
	if (!inner_ok(&o->ins[0]) || !inner_ok(&o->ins[1]))
		return 0;
	for (i = 0; i < 5; i++)
		if (o->s[i] != 0)
			return 0;
	return o->uc == 0 && o->b == 0 && o->e == E0;
}

static int local_ok(void)
{
	static struct outer loc;
	static int *lp;
	static double ld[2];

	return outer_ok(&loc) && lp == NULL && posz_d(ld[0]) && posz_d(ld[1]);
}

int main(void)
{
	int i;

	pc_begin("XBD.4.3-default-initialization.001");
	pc_test();
	pc_check(outer_ok(&g_o), "a static struct (pointer, floating members, nested aggregates, array, bool, enum) is not zero-initialized");
	pc_check(g_u.u == NULL, "the first named member of a static union (a pointer) is not null");
	pc_check(inner_ok(&g_v.in), "the first named member (a struct) of a static union is not default-initialized");
	pc_check(posz_d(g_w.first), "the first named member (a double) of a static union is not positive zero");
	for (i = 0; i < 2; i++)
		pc_check(g_hu[i].tag == 0 && g_hu[i].un.u == NULL && inner_ok(&g_hu[i].vn.in),
		    "static array element %d of structs holding unions is not default-initialized", i);
	for (i = 0; i < 3; i++)
		pc_check(g_pa[i] == NULL, "static pointer array element %d is not null", i);
	for (i = 0; i < 4; i++)
		pc_check(posz_d(g_da[i]), "static double array element %d is not positive zero", i);
	for (i = 0; i < 20; i++)
		pc_check(g_str[i] == 0, "static char array element %d is not zero", i);
	pc_check(posz_f(g_fa), "static float is not positive zero");
	pc_check(posz_l(g_lda), "static long double is not positive zero");
	pc_check(g_vp == NULL && g_fp == NULL, "a static object pointer or function pointer is not null");
	pc_check(g_uc == 0 && g_sh == 0 && g_ll == 0, "a static integer is not zero");
	pc_check(g_z == 0, "a static complex number is not zero");
	pc_check(tl_i == 0 && outer_ok(&tl_o), "a thread-storage object is not default-initialized");
	pc_check(ext_i == 0 && outer_ok(&ext_o), "an external object without initializer is not default-initialized");
	pc_check(local_ok(), "a function-scope static object is not default-initialized");
	pc_pass();
}
