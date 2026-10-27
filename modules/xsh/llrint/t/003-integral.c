/*
 * llrint() of an integral argument returns it.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.llrint.003
 */
#include "mt.h"

static void run_d(void)
{
	static const double in[] = { 0.0, (-0.0), 5.0, -5.0, 1048576.0, -1048576.0 };
	static const long long want[] = { 0LL, 0LL, 5LL, -5LL, 1048576LL, -1048576LL };
	volatile double x;
	volatile long long r;
	int i;

	for (i = 0; i < 6; i++) {
		x = in[i];
		r = llrint(x);
		pc_check(r == want[i], "llrint(%g) returned %lld, expected %lld", (double)x, (long long)r, (long long)want[i]);
	}
}

static void run_f(void)
{
	static const float in[] = { 0.0f, (-0.0f), 5.0f, -5.0f, 1048576.0f, -1048576.0f };
	static const long long want[] = { 0LL, 0LL, 5LL, -5LL, 1048576LL, -1048576LL };
	volatile float x;
	volatile long long r;
	int i;

	for (i = 0; i < 6; i++) {
		x = in[i];
		r = llrintf(x);
		pc_check(r == want[i], "llrintf(%g) returned %lld, expected %lld", (double)x, (long long)r, (long long)want[i]);
	}
}

static void run_l(void)
{
	static const long double in[] = { 0.0L, (-0.0L), 5.0L, -5.0L, 1048576.0L, -1048576.0L };
	static const long long want[] = { 0LL, 0LL, 5LL, -5LL, 1048576LL, -1048576LL };
	volatile long double x;
	volatile long long r;
	int i;

	for (i = 0; i < 6; i++) {
		x = in[i];
		r = llrintl(x);
		pc_check(r == want[i], "llrintl(%g) returned %lld, expected %lld", (double)x, (long long)r, (long long)want[i]);
	}
}

int main(void)
{
	pc_begin("XSH.llrint.003");
	pc_test();
	run_d();
	run_f();
	run_l();
	pc_pass();
}
