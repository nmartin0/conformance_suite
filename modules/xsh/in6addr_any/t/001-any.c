/*
 * in6addr_any holds ::.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.in6addr_any.001
 */
#include "pc.h"
#include <netinet/in.h>
#include <stdint.h>

int main(void)
{
	const uint8_t *b = in6addr_any.s6_addr;
	size_t i;

	pc_begin("XSH.in6addr_any.001");
	pc_test();
	for (i = 0; i < 16; i++)
		pc_check(b[i] == 0, "byte %zu of in6addr_any is %u, not 0", i, (unsigned)b[i]);
	pc_check(IN6_IS_ADDR_UNSPECIFIED(&in6addr_any), "IN6_IS_ADDR_UNSPECIFIED rejects in6addr_any");
	pc_pass();
}
