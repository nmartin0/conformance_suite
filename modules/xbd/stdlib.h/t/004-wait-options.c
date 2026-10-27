/*
 * WNOHANG and WUNTRACED are nonzero and do not share a bit.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.stdlib.h.004
 */
#include <stdlib.h>
#include "pc.h"

int main(void)
{
	pc_begin("XBD.stdlib.h.004");
	pc_test();
	pc_check(WNOHANG != 0, "WNOHANG is 0");
	pc_check(WUNTRACED != 0, "WUNTRACED is 0");
	pc_check((WNOHANG & WUNTRACED) == 0, "WNOHANG and WUNTRACED share a bit");
	pc_pass();
}
