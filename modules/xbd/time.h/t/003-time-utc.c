/*
 * TIME_UTC is a positive integer constant usable in #if.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.time.h.003
 */
#include <time.h>
#include "pc.h"

#if TIME_UTC <= 0
#error "TIME_UTC is not greater than 0 in #if"
#endif
_Static_assert(TIME_UTC > 0, "TIME_UTC is not a positive integer constant");

int main(void)
{
	pc_begin("XBD.time.h.003");
	pc_test();
	pc_pass();
}
