/*
 * BIG_ENDIAN and LITTLE_ENDIAN have distinct values.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.endian.h.002
 */
#include "pc.h"
#include <endian.h>

int main(void)
{
	pc_begin("XBD.endian.h.002");
#if !defined(LITTLE_ENDIAN) || !defined(BIG_ENDIAN)
	pc_unresolved("a macro is missing (the header check reports it): LITTLE_ENDIAN BIG_ENDIAN");
#else
	pc_test();
#if LITTLE_ENDIAN == BIG_ENDIAN
	pc_fail("LITTLE_ENDIAN and BIG_ENDIAN are both %lld", (long long)(LITTLE_ENDIAN));
#else
	pc_pass();
#endif
#endif
}
