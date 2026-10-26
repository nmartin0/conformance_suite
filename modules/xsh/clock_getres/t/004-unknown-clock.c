/*
 * A clock ID that names no clock: EINVAL from all three functions.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.clock_getres.004
 * MUTANT    clock_getres.unknown_ok
 * MUTANT    clock_getres.unknown_enosys
 * MUTANT    clock_getres.gettime_unknown_ok
 * MUTANT    clock_getres.settime_unknown_ok
 * VARIANT   clock_getres.extra_clock
 * VARIANT   clock_getres.settime_eperm
 */
#include "pc.h"
#include <errno.h>
#include <time.h>

/*
 * Candidates, none defined by any system we know of. Any of them may name
 * an additional clock (the implementation may have more): one that all
 * three functions accept is skipped.
 */
static const long candidates[] = { 0x7ffffff0L, 0x3ffffff7L, 0x12345679L, 0x5aa5a55aL };

int main(void)
{
	struct timespec ts, zero = { 0, 0 };
	size_t i;

	pc_begin("XSH.clock_getres.004");
	pc_test();
	for (i = 0; i < sizeof candidates / sizeof candidates[0]; i++) {
		clockid_t id = (clockid_t)candidates[i];
		int rg, eg, rt, et, rs, es;

		errno = 0;
		rg = clock_getres(id, &ts);
		eg = errno;
		errno = 0;
		rt = clock_gettime(id, &ts);
		et = errno;
		if (rg == 0 && rt == 0) {
			pc_note("clock ID %#lx is a known clock here; trying the next", candidates[i]);
			continue;
		}
		pc_check(rg == -1 && eg == EINVAL,
		    "clock_getres(%#lx) returned %d (%s), expected -1/EINVAL (clock_gettime gave %d, %s)",
		    candidates[i], rg, pc_errname(eg), rt, pc_errname(et));
		pc_check(rt == -1 && et == EINVAL,
		    "clock_gettime(%#lx) returned %d (%s), expected -1/EINVAL (clock_getres gave %d, %s)",
		    candidates[i], rt, pc_errname(et), rg, pc_errname(eg));
		/* An unknown clock: setting it can change nothing. */
		errno = 0;
		rs = clock_settime(id, &zero);
		es = errno;
		pc_check(rs == -1 && (es == EINVAL || es == EPERM),
		    "clock_settime(%#lx) returned %d (%s), expected -1/EINVAL (or EPERM)",
		    candidates[i], rs, pc_errname(es));
		pc_pass();
	}
	pc_unresolved("every candidate clock ID names a known clock here");
}
