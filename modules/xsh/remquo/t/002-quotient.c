/*
 * remquo() stores the sign and low bits of the integral quotient.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.remquo.002
 */
#include "mt.h"

static void run_d(void)
{
	static const double in1[] = { 9.0, -9.0, 9.0, -9.0, 10.0, -10.0, 17.0, 100.0, -100.0, 100.0 };
	static const double in2[] = { 4.0, 4.0, -4.0, -4.0, 2.0, 2.0, 5.0, 7.0, 7.0, -7.0 };
	static const int wq[] = { 2, -2, -2, 2, 5, -5, 3, 14, -14, -14 };
	volatile double x, y, r;
	int i, q, aq, wa;

	for (i = 0; i < 10; i++) {
		x = in1[i];
		y = in2[i];
		q = 12345;
		r = remquo(x, y, &q);
		(void)r;
		aq = q < 0 ? -q : q;
		wa = wq[i] < 0 ? -wq[i] : wq[i];
		pc_check((q < 0) == (wq[i] < 0) && q != 0, "remquo(%g, %g, &q) stored %d, which has the wrong sign", (double)x, (double)y, q);
		pc_check((aq & 7) == (wa & 7), "remquo(%g, %g, &q) stored %d, whose low three bits differ from those of the quotient %d", (double)x, (double)y, q, wq[i]);
	}
}

static void run_f(void)
{
	static const float in1[] = { 9.0f, -9.0f, 9.0f, -9.0f, 10.0f, -10.0f, 17.0f, 100.0f, -100.0f, 100.0f };
	static const float in2[] = { 4.0f, 4.0f, -4.0f, -4.0f, 2.0f, 2.0f, 5.0f, 7.0f, 7.0f, -7.0f };
	static const int wq[] = { 2, -2, -2, 2, 5, -5, 3, 14, -14, -14 };
	volatile float x, y, r;
	int i, q, aq, wa;

	for (i = 0; i < 10; i++) {
		x = in1[i];
		y = in2[i];
		q = 12345;
		r = remquof(x, y, &q);
		(void)r;
		aq = q < 0 ? -q : q;
		wa = wq[i] < 0 ? -wq[i] : wq[i];
		pc_check((q < 0) == (wq[i] < 0) && q != 0, "remquof(%g, %g, &q) stored %d, which has the wrong sign", (double)x, (double)y, q);
		pc_check((aq & 7) == (wa & 7), "remquof(%g, %g, &q) stored %d, whose low three bits differ from those of the quotient %d", (double)x, (double)y, q, wq[i]);
	}
}

static void run_l(void)
{
	static const long double in1[] = { 9.0L, -9.0L, 9.0L, -9.0L, 10.0L, -10.0L, 17.0L, 100.0L, -100.0L, 100.0L };
	static const long double in2[] = { 4.0L, 4.0L, -4.0L, -4.0L, 2.0L, 2.0L, 5.0L, 7.0L, 7.0L, -7.0L };
	static const int wq[] = { 2, -2, -2, 2, 5, -5, 3, 14, -14, -14 };
	volatile long double x, y, r;
	int i, q, aq, wa;

	for (i = 0; i < 10; i++) {
		x = in1[i];
		y = in2[i];
		q = 12345;
		r = remquol(x, y, &q);
		(void)r;
		aq = q < 0 ? -q : q;
		wa = wq[i] < 0 ? -wq[i] : wq[i];
		pc_check((q < 0) == (wq[i] < 0) && q != 0, "remquol(%g, %g, &q) stored %d, which has the wrong sign", (double)x, (double)y, q);
		pc_check((aq & 7) == (wa & 7), "remquol(%g, %g, &q) stored %d, whose low three bits differ from those of the quotient %d", (double)x, (double)y, q, wq[i]);
	}
}

int main(void)
{
	pc_begin("XSH.remquo.002");
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
