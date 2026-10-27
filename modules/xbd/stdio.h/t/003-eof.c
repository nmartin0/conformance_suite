/*
 * EOF is a negative int.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.stdio.h.003
 */
#include <stdio.h>
#include "pc.h"

_Static_assert(_Generic(EOF, int: 1, default: 0), "EOF does not have type int");
_Static_assert(EOF < 0, "EOF is not negative");

int main(void)
{
	pc_begin("XBD.stdio.h.003");
	pc_test();
	pc_pass();
}
