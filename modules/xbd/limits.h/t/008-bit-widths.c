/*
 * LONG_BIT and WORD_BIT agree with the ranges of long and int.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.limits.h.008
 */
#include <limits.h>
#include "pc.h"

#if LONG_BIT < 2 || WORD_BIT < 2
#error "LONG_BIT or WORD_BIT is below 2"
#endif
#if LONG_BIT <= 64
#if (LONG_MAX >> (LONG_BIT - 2)) != 1 || (LONG_MAX >> (LONG_BIT - 1)) != 0
#error "LONG_BIT does not match the range of long"
#endif
#endif
#if WORD_BIT <= 64
#if (INT_MAX >> (WORD_BIT - 2)) != 1 || (INT_MAX >> (WORD_BIT - 1)) != 0
#error "WORD_BIT does not match the range of int"
#endif
#endif

int main(void)
{
	pc_begin("XBD.limits.h.008");
	pc_test();
	pc_pass();
}
