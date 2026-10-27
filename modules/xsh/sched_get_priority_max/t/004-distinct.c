/*
 * The policy constants of <sched.h> are pairwise distinct.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sched_get_priority_max.004
 */
#include "pc.h"
#include <sched.h>

int main(void)
{
	int v[4], n = 0, i, j;

	pc_begin("XSH.sched_get_priority_max.004");
	pc_test();
	v[n++] = SCHED_FIFO;
	v[n++] = SCHED_RR;
	v[n++] = SCHED_OTHER;
#ifdef SCHED_SPORADIC
	v[n++] = SCHED_SPORADIC;
#endif
	for (i = 0; i < n; i++)
		for (j = i + 1; j < n; j++)
			pc_check(v[i] != v[j], "two policy constants share the value %d", v[i]);
	pc_pass();
}
