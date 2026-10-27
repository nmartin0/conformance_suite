/*
 * _IO* and SEEK_* constants are distinct.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.stdio.h.002
 */
#include <stdio.h>
#include "pc.h"

_Static_assert(_IOFBF != _IOLBF && _IOFBF != _IONBF && _IOLBF != _IONBF, "two of _IOFBF, _IOLBF and _IONBF are equal");
_Static_assert(SEEK_CUR != SEEK_END && SEEK_CUR != SEEK_SET && SEEK_END != SEEK_SET, "two of SEEK_CUR, SEEK_END and SEEK_SET are equal");

int main(void)
{
	pc_begin("XBD.stdio.h.002");
	pc_test();
	pc_pass();
}
