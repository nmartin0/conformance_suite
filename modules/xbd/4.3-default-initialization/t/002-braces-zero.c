/*
 * An initializer of { 0 } default-initializes an aggregate (or a scalar)
 * whatever its storage duration, and whatever the memory held before.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.4.3-default-initialization.002
 */
#include "pc.h"
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

struct inner { float f; void *q; int arr[3]; };
union U { void *u; double dd; };
struct outer {
	int *p;
	double d;
	struct inner in;
	struct inner ins[2];
	char s[5];
	union U un;
	_Bool b;
};

static int posz_d(double x) { return x == 0.0 && !signbit(x); }

static int inner_ok(const struct inner *p)
{
	int i;

	if (!(p->f == 0.0f && !signbit(p->f)) || p->q != NULL)
		return 0;
	for (i = 0; i < 3; i++)
		if (p->arr[i] != 0)
			return 0;
	return 1;
}

static int outer_ok(const struct outer *o)
{
	int i;

	if (o->p != NULL || !posz_d(o->d) || !inner_ok(&o->in) || !inner_ok(&o->ins[0]) || !inner_ok(&o->ins[1]))
		return 0;
	for (i = 0; i < 5; i++)
		if (o->s[i] != 0)
			return 0;
	return o->un.u == NULL && o->b == 0;
}

/* Leaves a pattern in the stack area that the next call is likely to use. */
static void dirty(void)
{
	volatile unsigned char buf[2048];
	size_t i;

	for (i = 0; i < sizeof buf; i++)
		buf[i] = 0xA5;
}

static int auto_struct(void)
{
	struct outer o = { 0 };

	return outer_ok(&o);
}

static int auto_array(void)
{
	struct inner a[3] = { 0 };

	return inner_ok(&a[0]) && inner_ok(&a[1]) && inner_ok(&a[2]);
}

static int auto_scalar_array(void)
{
	int v[8] = { 0 };
	double w[3] = { 0 };
	void *q[2] = { 0 };
	int i;

	for (i = 0; i < 8; i++)
		if (v[i] != 0)
			return 0;
	return posz_d(w[0]) && posz_d(w[1]) && posz_d(w[2]) && q[0] == NULL && q[1] == NULL;
}

static int static_struct(void)
{
	static struct outer s = { 0 };

	return outer_ok(&s);
}

static _Thread_local struct outer tl = { 0 };

static int literal_struct(void)
{
	struct outer *h = malloc(sizeof *h);
	int ok;

	if (h == NULL)
		return -1;
	memset(h, 0xA5, sizeof *h);
	*h = (struct outer){ 0 };
	ok = outer_ok(h);
	free(h);
	return ok;
}

static int auto_in_loop(void)
{
	int i;

	for (i = 0; i < 3; i++) {
		struct outer o = { 0 };
		int ok = outer_ok(&o);

		o.d = 1.5;			/* change it; the next pass must start over */
		o.in.arr[1] = 7;
		o.s[2] = 'x';
		if (!ok)
			return 0;
	}
	return 1;
}

int main(void)
{
	int r;

	pc_begin("XBD.4.3-default-initialization.002");
	pc_test();
	dirty();
	pc_check(auto_struct(), "an automatic struct initialized with { 0 } is not default-initialized");
	dirty();
	pc_check(auto_array(), "an automatic array of structs initialized with { 0 } is not default-initialized");
	dirty();
	pc_check(auto_scalar_array(), "automatic scalar arrays initialized with { 0 } are not default-initialized");
	pc_check(static_struct(), "a static struct initialized with { 0 } is not default-initialized");
	pc_check(outer_ok(&tl), "a thread-storage struct initialized with { 0 } is not default-initialized");
	r = literal_struct();
	if (r < 0)
		pc_unresolved("malloc failed");
	pc_check(r, "a compound literal { 0 } assigned over dirty memory is not default-initialized");
	dirty();
	pc_check(auto_in_loop(), "an automatic struct re-initialized with { 0 } on each pass of a loop is not default-initialized");
	pc_pass();
}
