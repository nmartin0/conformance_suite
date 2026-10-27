/*
 * atomic_store() replaces the value of the object, for every atomic
 * integer type (extreme values included).
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.atomic_store.001
 */
#define PCAT_ID "XSH.atomic_store.001"
#include "pcat.h"
#ifndef PCAT_NONE

/*
 * The object is read plainly (an ordinary read of an atomic object is
 * itself an atomic load) so that atomic_load() is not relied on.
 */
#define CASE(tag, ct, mn, mx) \
static void run_##tag(void) \
{ \
	ct vals[4] = { (ct)0, (ct)1, (ct)(mn), (ct)(mx) }; \
	int i; \
	for (i = 0; i < 4; i++) { \
		atomic_##tag obj = vals[(i + 1) % 4]; \
		atomic_##tag *volatile p = &obj; \
		ct got; \
		atomic_store(p, vals[i]); \
		got = *p; \
		pc_check(got == vals[i], "atomic_" #tag ": object holds %llx after storing %llx", \
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
