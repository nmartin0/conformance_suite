/*
 * BYTE_ORDER, LITTLE_ENDIAN and BIG_ENDIAN are defined, usable in #if,
 * and BYTE_ORDER equals one of the *_ENDIAN macros.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.endian.h.001
 */
#include "pc.h"
#include <endian.h>

int main(void)
{
	pc_begin("XBD.endian.h.001");
#if !defined(BYTE_ORDER) || !defined(LITTLE_ENDIAN) || !defined(BIG_ENDIAN)
	pc_unresolved("a macro is missing (the header check reports it): BYTE_ORDER LITTLE_ENDIAN BIG_ENDIAN");
#else
	pc_test();
#if BYTE_ORDER == LITTLE_ENDIAN
	pc_pass();
#elif BYTE_ORDER == BIG_ENDIAN
	pc_pass();
#elif defined(PDP_ENDIAN) && BYTE_ORDER == PDP_ENDIAN
	pc_pass();
#else
	/* Other *_ENDIAN names are allowed and cannot be listed here; a value
	 * that matches none of the three known ones is not proof of a fault. */
	pc_unresolved("BYTE_ORDER equals none of LITTLE_ENDIAN, BIG_ENDIAN, PDP_ENDIAN; "
	    "it may equal an *_ENDIAN macro this test does not know");
#endif
#endif
}
