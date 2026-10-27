/*
 * A store changes only the object it names: its neighbours in an array
 * of atomic objects keep their values, for both bit patterns.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.atomic_store.004
 */
#define PCAT_ID "XSH.atomic_store.004"
#include "pcat.h"
#ifndef PCAT_NONE

#define CASE(tag, ct, mn, mx) \
static void run_##tag(void) \
{ \
	atomic_##tag arr[5]; \
	atomic_##tag *volatile p = arr; \
	ct fill[2] = { (ct)(mx), (ct)(mn) }; \
	ct put[2] = { (ct)(mn), (ct)(mx) }; \
	int ph, k; \
	for (ph = 0; ph < 2; ph++) { \
		for (k = 0; k < 5; k++) \
			arr[k] = fill[ph]; \
		atomic_store(&p[2], put[ph]); \
		for (k = 0; k < 5; k++) { \
			ct got = p[k]; \
			ct want = k == 2 ? put[ph] : fill[ph]; \
			pc_check(got == want, "atomic_" #tag " phase %d: element %d holds %llx, expected %llx", \
			    ph, k, PCAT_V(got), PCAT_V(want)); \
		} \
		atomic_store_explicit(&p[3], put[ph], memory_order_release); \
		for (k = 0; k < 5; k++) { \
			ct got = p[k]; \
			ct want = (k == 2 || k == 3) ? put[ph] : fill[ph]; \
			pc_check(got == want, "atomic_" #tag " phase %d (explicit): element %d holds %llx, expected %llx", \
			    ph, k, PCAT_V(got), PCAT_V(want)); \
		} \
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
