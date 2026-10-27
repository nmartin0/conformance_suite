/*
 * in_port_t and in_addr_t are the 16-bit and 32-bit unsigned types;
 *   uint8_t and uint32_t come from this header.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.netinet_in.h.001
 */
#include <netinet/in.h>
#include "pc.h"

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

/* uint8_t and uint32_t have to come from <netinet/in.h> itself. */
static uint8_t octet = 255;
static uint32_t word = 1;

#include <inttypes.h>

int main(void)
{
	struct in_addr a;
	in_port_t p = 0;
	in_addr_t q = 0;

	pc_begin("XBD.netinet_in.h.001");
	pc_test();
	pc_check(octet == 255 && word == 1, "uint8_t or uint32_t misbehaves");
	pc_check(IS_T(p, uint16_t), "in_port_t is not uint16_t");
	pc_check(IS_T(q, uint32_t), "in_addr_t is not uint32_t");
	pc_check((sa_family_t)-1 > 0 && (sa_family_t)1.9 == 1, "sa_family_t is not an unsigned integer type");
	pc_check(IS_T(a.s_addr, in_addr_t), "s_addr does not have type in_addr_t");
	pc_pass();
}
