/*
 * The CLOCK_* identifiers fit clockid_t and are pairwise different.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.time.h.004
 */
#include <time.h>
#include "pc.h"

int main(void)
{
	pc_begin("XBD.time.h.004");
	pc_test();
	static const struct { const char *name; clockid_t id; } c[] = {
		{ "CLOCK_REALTIME", CLOCK_REALTIME },
		{ "CLOCK_MONOTONIC", CLOCK_MONOTONIC },
#ifdef CLOCK_PROCESS_CPUTIME_ID
		{ "CLOCK_PROCESS_CPUTIME_ID", CLOCK_PROCESS_CPUTIME_ID },
#endif
#ifdef CLOCK_THREAD_CPUTIME_ID
		{ "CLOCK_THREAD_CPUTIME_ID", CLOCK_THREAD_CPUTIME_ID },
#endif
	};
	size_t i, j;
	clockid_t x = CLOCK_REALTIME;

	pc_check(x == CLOCK_REALTIME, "CLOCK_REALTIME does not survive assignment to clockid_t");
	for (i = 0; i < sizeof c / sizeof c[0]; i++) {
		pc_check(c[i].id == (clockid_t)c[i].id, "%s is not a clockid_t value", c[i].name);
		for (j = i + 1; j < sizeof c / sizeof c[0]; j++)
			pc_check(c[i].id != c[j].id, "%s and %s have the same value", c[i].name, c[j].name);
	}
	pc_pass();
}
