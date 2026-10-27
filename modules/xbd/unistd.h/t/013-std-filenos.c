/*
 * The standard file descriptor numbers.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.unistd.h.013
 */
#include <unistd.h>
#include "pc.h"

_Static_assert(STDIN_FILENO == 0, "STDIN_FILENO is not 0");
_Static_assert(STDOUT_FILENO == 1, "STDOUT_FILENO is not 1");
_Static_assert(STDERR_FILENO == 2, "STDERR_FILENO is not 2");

int main(void)
{
	pc_begin("XBD.unistd.h.013");
	pc_test();
	pc_pass();
}
