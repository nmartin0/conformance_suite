/*
 * CLOCK_REALTIME and CLOCK_MONOTONIC have a resolution of 20 ms or finer.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.8-realtime.002
 */
#include "pc.h"
#include <errno.h>
#include <limits.h>
#include <time.h>

#if !defined(_POSIX_CLOCKRES_MIN)
#error "_POSIX_CLOCKRES_MIN is not defined by <limits.h>"
#endif
#if (_POSIX_CLOCKRES_MIN) != 20000000
#error "_POSIX_CLOCKRES_MIN is not 20000000"
#endif

int main(void)
{
	struct timespec res;
	static const struct { clockid_t id; const char *name; } clocks[] = {
		{ CLOCK_REALTIME, "CLOCK_REALTIME" }, { CLOCK_MONOTONIC, "CLOCK_MONOTONIC" }
	};
	unsigned i;

	pc_begin("XSH.2.8-realtime.002");
	pc_test();
	pc_check(_POSIX_CLOCKRES_MIN == 20000000L, "_POSIX_CLOCKRES_MIN is not 20000000");
	for (i = 0; i < sizeof clocks / sizeof clocks[0]; i++) {
		pc_check(clock_getres(clocks[i].id, &res) == 0, "clock_getres(%s) failed: %s", clocks[i].name,
		    pc_errname(errno));
		pc_check(res.tv_sec == 0 && res.tv_nsec > 0 && res.tv_nsec <= 20000000L,
		    "the resolution of %s is %ld s %ld ns, coarser than 20 ms or not positive", clocks[i].name,
		    (long)res.tv_sec, res.tv_nsec);
	}
	pc_pass();
}
