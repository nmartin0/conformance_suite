/*
 * IPPROTO_IP, IPPROTO_ICMP, IPPROTO_TCP, IPPROTO_UDP and SOL_SOCKET are
 *   all different.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.netinet_in.h.002
 */
#include <netinet/in.h>
#include <sys/socket.h>
#include "pc.h"


/* name, value, missing */
static const struct { const char *name; long long value; int missing; } v[] = {
#ifdef IPPROTO_IP
	{ "IPPROTO_IP", (long long)(IPPROTO_IP), 0 },
#else
	{ "IPPROTO_IP", 0, 1 },
#endif
#ifdef IPPROTO_ICMP
	{ "IPPROTO_ICMP", (long long)(IPPROTO_ICMP), 0 },
#else
	{ "IPPROTO_ICMP", 0, 1 },
#endif
#ifdef IPPROTO_TCP
	{ "IPPROTO_TCP", (long long)(IPPROTO_TCP), 0 },
#else
	{ "IPPROTO_TCP", 0, 1 },
#endif
#ifdef IPPROTO_UDP
	{ "IPPROTO_UDP", (long long)(IPPROTO_UDP), 0 },
#else
	{ "IPPROTO_UDP", 0, 1 },
#endif
#ifdef SOL_SOCKET
	{ "SOL_SOCKET", (long long)(SOL_SOCKET), 0 },
#else
	{ "SOL_SOCKET", 0, 1 },
#endif
};

int main(void)
{
	size_t i, j, n = sizeof v / sizeof v[0];

	pc_begin("XBD.netinet_in.h.002");
	pc_test();
	for (i = 0; i < n; i++) {
		pc_check(!v[i].missing, "the headers do not define %s", v[i].name);
		for (j = i + 1; j < n; j++)
			pc_check(v[i].missing || v[j].missing || v[i].value != v[j].value, "%s and %s are both %lld", v[i].name, v[j].name, v[i].value);
	}
	pc_pass();
}
