/*
 * An initializer of { 0 } on a scalar gives it the default value: zero, a
 * null pointer, positive zero for floating types.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.4.3-default-initialization.005
 */
#include "pc.h"
#include <math.h>
#include <stddef.h>

enum color { RED, GREEN };

static void dirty(void)
{
	volatile unsigned char buf[1024];
	unsigned i;

	for (i = 0; i < sizeof buf; i++)
		buf[i] = 0xA5;
}

static int scalars(void)
{
	int i = { 0 };
	unsigned long ul = { 0 };
	char c = { 0 };
	_Bool b = { 0 };
	enum color e = { 0 };
	double d = { 0 };
	float f = { 0 };
	long double ld = { 0 };
	int *p = { 0 };
	void (*fp)(void) = { 0 };
	const char *s = { 0 };

	return i == 0 && ul == 0 && c == 0 && b == 0 && e == RED && d == 0.0 && !signbit(d) &&
	    f == 0.0f && !signbit(f) && ld == 0.0L && !signbit(ld) && p == NULL && fp == NULL && s == NULL;
}

static int statics(void)
{
	static double d = { 0 };
	static int *p = { 0 };
	static long n = { 0 };

	return d == 0.0 && !signbit(d) && p == NULL && n == 0;
}

int main(void)
{
	pc_begin("XBD.4.3-default-initialization.005");
	pc_test();
	dirty();
	pc_check(scalars(), "an automatic scalar initialized with { 0 } does not hold zero, a null pointer or positive zero");
	pc_check(statics(), "a static scalar initialized with { 0 } does not hold zero, a null pointer or positive zero");
	pc_pass();
}
