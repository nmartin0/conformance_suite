/*
 * EXIT_SUCCESS and EXIT_FAILURE values.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.stdlib.h.001
 */
#include <stdlib.h>
#include "pc.h"

_Static_assert(EXIT_SUCCESS == 0, "EXIT_SUCCESS is not 0");
_Static_assert(EXIT_FAILURE >= 1 && EXIT_FAILURE <= 125, "EXIT_FAILURE is not between 1 and 125");

int main(void)
{
	pc_begin("XBD.stdlib.h.001");
	pc_test();
	pc_pass();
}
