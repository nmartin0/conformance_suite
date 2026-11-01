/*
 * TCP_NODELAY is defined as an integer constant that fits the int
 * option-name argument of a socket-option call.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.netinet_tcp.h.001
 */
#include "pc.h"
#include <limits.h>
#include <netinet/tcp.h>

#ifdef TCP_NODELAY
enum { pc_nodelay = 0 * (TCP_NODELAY) };	/* an integer constant expression */
#endif

int main(void)
{
	pc_begin("XBD.netinet_tcp.h.001");
#ifndef TCP_NODELAY
	pc_unresolved("TCP_NODELAY is missing (the header check reports it)");
#else
	pc_test();
	(void)pc_nodelay;
#if (TCP_NODELAY) > INT_MAX || (TCP_NODELAY) < INT_MIN
	pc_fail("TCP_NODELAY does not fit an int");
#endif
	pc_pass();
#endif
}
