/*
 * rlim_t can hold every non-negative value of int and of off_t.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.11-data-types.005
 */
#include "pc.h"
#include <limits.h>
#include <sys/resource.h>
#include <sys/types.h>

#define ISSIGNED(T) (!((T)~(T)0 > (T)0))

/* The largest value of T, found by growing a run of one bits (no assumption about width or padding). */
#define DEFMAX(T) \
static unsigned long long max_##T(void) \
{ \
	unsigned long long m = 0, c; \
	int k; \
	for (k = 0; k < 64; k++) { \
		c = (m << 1) | 1; \
		if ((unsigned long long)(T)c != c || (ISSIGNED(T) && (T)c <= 0)) \
			break; \
		m = c; \
	} \
	return m; \
}
DEFMAX(rlim_t)
DEFMAX(off_t)

int main(void)
{
	pc_begin("XSH.2.11-data-types.005");
	pc_test();
	pc_check(max_rlim_t() >= (unsigned long long)INT_MAX, "the largest rlim_t, %llu, is below INT_MAX", max_rlim_t());
	pc_check(max_rlim_t() >= max_off_t(), "the largest rlim_t, %llu, is below the largest off_t, %llu", max_rlim_t(),
	    max_off_t());
	{
		volatile off_t o = (off_t)max_off_t();
		volatile int n = INT_MAX;
		rlim_t r1 = (rlim_t)o, r2 = (rlim_t)n;

		pc_check((off_t)r1 == o, "the largest off_t does not survive a cast to rlim_t and back");
		pc_check((int)r2 == n, "INT_MAX does not survive a cast to rlim_t and back");
	}
	pc_pass();
}
