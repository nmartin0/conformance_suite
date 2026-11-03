/*
 * When BYTE_ORDER equals LITTLE_ENDIAN (BIG_ENDIAN) the integer types of
 * this host store the least (most) significant byte first.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.endian.h.003
 */
#include "pc.h"
#include <endian.h>
#include <stdint.h>

/* Index of the byte of v (n bytes) that holds the given value, or -1. */
static int where(const void *v, size_t n, unsigned char b)
{
	const unsigned char *p = v;
	size_t i;

	for (i = 0; i < n; i++)
		if (p[i] == b)
			return (int)i;
	return -1;
}

int main(void)
{
	uint16_t a = 0x0102;
	uint32_t b = 0x01020304UL;
	uint64_t c = 0x0102030405060708ULL;
	int order;			/* 1: least significant first; 2: most; 0: neither claimed */

	pc_begin("XBD.endian.h.003");
#if !defined(BYTE_ORDER) || !defined(LITTLE_ENDIAN) || !defined(BIG_ENDIAN)
	pc_unresolved("a macro is missing (the header check reports it): BYTE_ORDER LITTLE_ENDIAN BIG_ENDIAN");
#else
#if BYTE_ORDER == LITTLE_ENDIAN
	order = 1;
#elif BYTE_ORDER == BIG_ENDIAN
	order = 2;
#else
	order = 0;
#endif
	if (order == 0)
		pc_unresolved("BYTE_ORDER is neither LITTLE_ENDIAN nor BIG_ENDIAN; the page gives no layout for another value");
	pc_test();
	if (order == 1) {
		pc_check(where(&a, sizeof a, 0x02) == 0 && where(&a, sizeof a, 0x01) == 1,
		    "BYTE_ORDER == LITTLE_ENDIAN but a 16-bit integer does not keep its low byte first");
		pc_check(where(&b, sizeof b, 0x04) == 0 && where(&b, sizeof b, 0x01) == 3,
		    "BYTE_ORDER == LITTLE_ENDIAN but a 32-bit integer does not keep its low byte first");
		pc_check(where(&c, sizeof c, 0x08) == 0 && where(&c, sizeof c, 0x01) == 7,
		    "BYTE_ORDER == LITTLE_ENDIAN but a 64-bit integer does not keep its low byte first");
	} else {
		pc_check(where(&a, sizeof a, 0x01) == 0 && where(&a, sizeof a, 0x02) == 1,
		    "BYTE_ORDER == BIG_ENDIAN but a 16-bit integer does not keep its high byte first");
		pc_check(where(&b, sizeof b, 0x01) == 0 && where(&b, sizeof b, 0x04) == 3,
		    "BYTE_ORDER == BIG_ENDIAN but a 32-bit integer does not keep its high byte first");
		pc_check(where(&c, sizeof c, 0x01) == 0 && where(&c, sizeof c, 0x08) == 7,
		    "BYTE_ORDER == BIG_ENDIAN but a 64-bit integer does not keep its high byte first");
	}
	pc_pass();
#endif
}
