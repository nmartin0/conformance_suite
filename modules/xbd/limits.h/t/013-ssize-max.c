/*
 * SSIZE_MAX is the maximum of ssize_t.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.limits.h.013
 */
#include <sys/types.h>
#include <limits.h>
#include "pc.h"

#if SSIZE_MAX < _POSIX_SSIZE_MAX
#error "SSIZE_MAX is below _POSIX_SSIZE_MAX"
#endif
_Static_assert(sizeof(ssize_t) * CHAR_BIT <= 64, "ssize_t is wider than the test handles");
_Static_assert((unsigned long long)SSIZE_MAX == ((1ULL << (sizeof(ssize_t) * CHAR_BIT - 1)) - 1), "SSIZE_MAX is not the maximum of ssize_t");
_Static_assert((ssize_t)-1 < 0, "ssize_t is not signed");

int main(void)
{
	pc_begin("XBD.limits.h.013");
	pc_test();
	pc_pass();
}
