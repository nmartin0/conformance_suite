/*
 * Subobjects of an aggregate that its initializer does not name are
 * default-initialized: null pointers, positive zero, the first named member
 * of a union, the rest of an array.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.4.3-default-initialization.004
 */
#include "pc.h"
#include <math.h>
#include <stddef.h>

struct inner { float f; void *q; };
union U { void *u; double dd; };
union V { double first; char c[24]; };
struct T {
	int tag;
	int *p;
	double d;
	char s[8];
	struct inner in;
	struct inner arr[2];
	union U un;
	union V vn;
	long double ld;
};

static void dirty(void)
{
	volatile unsigned char buf[2048];
	size_t i;

	for (i = 0; i < sizeof buf; i++)
		buf[i] = 0xA5;
}

static int rest_ok(const struct T *t)
{
	int i;

	if (t->p != NULL || !(t->d == 0.0 && !signbit(t->d)))
		return 0;
	for (i = 2; i < 8; i++)
		if (t->s[i] != 0)
			return 0;
	if (!(t->in.f == 0.0f && !signbit(t->in.f)) || t->in.q != NULL)
		return 0;
	for (i = 0; i < 2; i++)
		if (!(t->arr[i].f == 0.0f && !signbit(t->arr[i].f)) || t->arr[i].q != NULL)
			return 0;
	if (t->un.u != NULL)
		return 0;
	if (!(t->vn.first == 0.0 && !signbit(t->vn.first)))
		return 0;
	return t->ld == 0.0L && !signbit(t->ld);
}

static int by_name(void)
{
	struct T t = { .tag = 4, .s = "ab" };

	return t.tag == 4 && t.s[0] == 'a' && t.s[1] == 'b' && rest_ok(&t);
}

static int by_position(void)
{
	int k = 3;
	struct T t = { 1, &k, 2.5 };

	return t.tag == 1 && t.p == &k && t.d == 2.5 && t.s[0] == 0 && t.s[1] == 0 &&
	    t.in.q == NULL && t.arr[1].q == NULL && t.un.u == NULL &&
	    t.vn.first == 0.0 && !signbit(t.vn.first) && t.ld == 0.0L && !signbit(t.ld);
}

static int nested_partial(void)
{
	struct T t = { .in = { .f = 1.0f }, .arr = { [1] = { .q = &t } } };

	return t.in.f == 1.0f && t.in.q == NULL && t.arr[0].q == NULL &&
	    t.arr[0].f == 0.0f && t.arr[1].f == 0.0f && t.arr[1].q == &t && t.tag == 0 && t.p == NULL &&
	    t.un.u == NULL;
}

int main(void)
{
	pc_begin("XBD.4.3-default-initialization.004");
	pc_test();
	dirty();
	pc_check(by_name(), "unnamed subobjects of a struct initialized with designators are not default-initialized");
	dirty();
	pc_check(by_position(), "unnamed trailing members of a struct initialized positionally are not default-initialized");
	dirty();
	pc_check(nested_partial(), "partially initialized nested aggregates do not default-initialize the rest");
	pc_pass();
}
