/*
 * atomic_load() returns the value the object holds, for every atomic
 * integer type (extreme values included).
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.atomic_load.001
 */
#define PCAT_ID "XSH.atomic_load.001"
#include "pcat.h"
#ifndef PCAT_NONE

#define CASE(tag, ct, mn, mx) \
static void run_##tag(void) \
{ \
	ct vals[4] = { (ct)0, (ct)1, (ct)(mn), (ct)(mx) }; \
	int i; \
	for (i = 0; i < 4; i++) { \
		atomic_##tag obj = vals[i]; \
		atomic_##tag *volatile p = &obj; \
		ct got = atomic_load(p); \
		pc_check(got == vals[i], "atomic_" #tag ": loaded %llx, object held %llx", \
		    PCAT_V(got), PCAT_V(vals[i])); \
	} \
}
PCAT_ALL(CASE)
#undef CASE

int main(void)
{
	pc_begin(PCAT_ID);
	pc_test();
#define CALL(tag, ct, mn, mx) run_##tag();
	PCAT_ALL(CALL)
#undef CALL
	pc_pass();
}
#endif
