/*
 * The lockf() function codes differ.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.unistd.h.017
 */
#include <unistd.h>
#include "pc.h"

_Static_assert(F_LOCK != F_TEST && F_LOCK != F_TLOCK && F_LOCK != F_ULOCK && F_TEST != F_TLOCK && F_TEST != F_ULOCK && F_TLOCK != F_ULOCK,
	"two of F_LOCK, F_TEST, F_TLOCK and F_ULOCK are equal");

int main(void)
{
	pc_begin("XBD.unistd.h.017");
	pc_test();
	pc_pass();
}
