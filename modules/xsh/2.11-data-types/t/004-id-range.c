/*
 * id_t can hold every value of pid_t, uid_t and gid_t.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.11-data-types.004
 */
#include "pc.h"
#include <sys/types.h>

#define ISSIGNED(T) (!((T)~(T)0 > (T)0))

/*
 * The largest value of T, found by growing a run of one bits until T can no
 * longer hold it (no assumption about the width or padding of T).
 */
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
} \
/* The smallest value of T: 0, or minus the largest minus one (two's complement). */ \
static long long min_##T(void) \
{ \
	return ISSIGNED(T) ? -(long long)max_##T() - 1 : 0LL; \
}
DEFMAX(id_t)
DEFMAX(pid_t)
DEFMAX(uid_t)
DEFMAX(gid_t)


#define CHECKMAX(T) pc_check(max_id_t() >= max_##T(), "the largest id_t, %llu, is below the largest " #T ", %llu", \
	max_id_t(), max_##T())
#define CHECKMIN(T) pc_check(min_id_t() <= min_##T(), "the smallest id_t, %lld, is above the smallest " #T ", %lld", \
	min_id_t(), min_##T())

int main(void)
{
	pc_begin("XSH.2.11-data-types.004");
	pc_test();
	pc_check(max_id_t() > 0 && max_pid_t() > 0 && max_uid_t() > 0 && max_gid_t() > 0, "a type has no positive value");
	CHECKMAX(pid_t);
	CHECKMAX(uid_t);
	CHECKMAX(gid_t);
	CHECKMIN(pid_t);
	CHECKMIN(uid_t);
	CHECKMIN(gid_t);
	pc_pass();
}
