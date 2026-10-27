/*
 * RAND_MAX is at least 32767.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.stdlib.h.002
 */
#include <stdlib.h>
#include "pc.h"

_Static_assert(RAND_MAX >= 32767, "RAND_MAX is below 32767");

int main(void)
{
	pc_begin("XBD.stdlib.h.002");
	pc_test();
	pc_pass();
}
