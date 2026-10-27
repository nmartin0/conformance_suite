/*
 * <arpa/inet.h> defines in_port_t, in_addr_t, socklen_t, struct in_addr
 *   and INET_ADDRSTRLEN.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.arpa_inet.h.002
 */
#include <arpa/inet.h>
#include <limits.h>
#include "pc.h"

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

int main(void)
{
	struct in_addr a;

	pc_begin("XBD.arpa_inet.h.002");
	pc_test();
	pc_check((in_port_t)-1 > 0 && sizeof(in_port_t) == 2, "in_port_t is not a 16-bit unsigned type");
	pc_check((in_addr_t)-1 > 0 && sizeof(in_addr_t) == 4, "in_addr_t is not a 32-bit unsigned type");
	pc_check((socklen_t)1.9 == 1 && sizeof(socklen_t) * CHAR_BIT >= 32, "socklen_t is not an integer type of at least 32 bits");
	pc_check(IS_T(a.s_addr, in_addr_t), "s_addr does not have type in_addr_t");
#ifndef INET_ADDRSTRLEN
	pc_fail("<arpa/inet.h> does not define INET_ADDRSTRLEN");
#else
	pc_check(INET_ADDRSTRLEN == 16, "INET_ADDRSTRLEN is not 16");
#endif
	pc_pass();
}
