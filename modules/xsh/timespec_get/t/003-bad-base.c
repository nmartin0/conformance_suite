/*
 * timespec_get() with base 0.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.timespec_get.003
 */
#include "pc.h"
#include <time.h>

int main(void)
{
	struct timespec ts;
	int r;

	pc_begin("XSH.timespec_get.003");
	pc_test();
	ts.tv_sec = 0;
	ts.tv_nsec = 0;
	r = timespec_get(&ts, 0);
	pc_check(r == 0, "timespec_get(&ts, 0) returned %d, expected 0", r);
	pc_pass();
}
