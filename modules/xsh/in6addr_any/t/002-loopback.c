/*
 * in6addr_loopback holds ::1.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.in6addr_any.002
 */
#include "pc.h"
#include <netinet/in.h>
#include <stdint.h>

int main(void)
{
	const uint8_t *b = in6addr_loopback.s6_addr;
	size_t i;

	pc_begin("XSH.in6addr_any.002");
	pc_test();
	for (i = 0; i < 15; i++)
		pc_check(b[i] == 0, "byte %zu of in6addr_loopback is %u, not 0", i, (unsigned)b[i]);
	pc_check(b[15] == 1, "byte 15 of in6addr_loopback is %u, not 1", (unsigned)b[15]);
	pc_check(IN6_IS_ADDR_LOOPBACK(&in6addr_loopback), "IN6_IS_ADDR_LOOPBACK rejects in6addr_loopback");
	pc_pass();
}
