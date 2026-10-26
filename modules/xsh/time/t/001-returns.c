/*
 * time() returns the time and stores it through tloc.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.time.001
 * MUTANT    time.minus_one
 * MUTANT    time.tloc_untouched
 * MUTANT    time.tloc_other
 * MUTANT    time.null_fails
 */
#include "pc.h"
#include <errno.h>
#include <string.h>
#include <time.h>

int main(void)
{
	time_t t, v, n;

	pc_begin("XSH.time.001");
	memset(&t, 0x5a, sizeof t);
	pc_test();
	v = time(&t);
	pc_check(v != (time_t)-1, "time(&t) returned (time_t)-1 (%s)", pc_errname(errno));
	pc_check(t == v, "time(&t) returned %lld but stored %lld", (long long)v, (long long)t);
	n = time(NULL);
	pc_check(n != (time_t)-1, "time(NULL) returned (time_t)-1 (%s)", pc_errname(errno));
	pc_pass();
}
