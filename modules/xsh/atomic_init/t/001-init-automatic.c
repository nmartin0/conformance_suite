/*
 * atomic_init() gives an automatic atomic object the value, for every
 * atomic integer type, even when the storage held junk (including the
 * page's example).
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.atomic_init.001
 */
#define PCAT_ID "XSH.atomic_init.001"
#include "pcat.h"
#ifndef PCAT_NONE

#define CASE(tag, ct, mn, mx) \
static void run_##tag(void) \
{ \
	ct vals[4] = { (ct)0, (ct)1, (ct)(mn), (ct)(mx) }; \
	int i; \
	for (i = 0; i < 4; i++) { \
		atomic_##tag obj; \
		atomic_##tag *volatile p = &obj; \
		ct got, got2; \
		memset((void *)&obj, 0xa5, sizeof obj); \
		atomic_init(p, vals[i]); \
		got = *p; \
		got2 = atomic_load(p); \
		pc_check(got == vals[i], "atomic_" #tag ": object holds %llx after atomic_init(%llx)", \
		    PCAT_V(got), PCAT_V(vals[i])); \
		pc_check(got2 == vals[i], "atomic_" #tag ": atomic_load gave %llx after atomic_init(%llx)", \
		    PCAT_V(got2), PCAT_V(vals[i])); \
	} \
}
PCAT_ALL(CASE)
#undef CASE

int main(void)
{
	atomic_int guide;
	atomic_int *volatile pg = &guide;

	pc_begin(PCAT_ID);
	pc_test();
#define CALL(tag, ct, mn, mx) run_##tag();
	PCAT_ALL(CALL)
#undef CALL
	atomic_init(&guide, 42);
	pc_check(atomic_load(pg) == 42, "the example (atomic_init(&guide, 42)) did not give 42");
	pc_pass();
}
#endif
