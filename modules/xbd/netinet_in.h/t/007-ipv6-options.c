/*
 * The IPV6_ socket options have distinct integer values.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.netinet_in.h.007
 */
#include <netinet/in.h>
#include "pc.h"


/* name, value, missing */
static const struct { const char *name; long long value; int missing; } v[] = {
#ifdef IPV6_JOIN_GROUP
	{ "IPV6_JOIN_GROUP", (long long)(IPV6_JOIN_GROUP), 0 },
#else
	{ "IPV6_JOIN_GROUP", 0, 1 },
#endif
#ifdef IPV6_LEAVE_GROUP
	{ "IPV6_LEAVE_GROUP", (long long)(IPV6_LEAVE_GROUP), 0 },
#else
	{ "IPV6_LEAVE_GROUP", 0, 1 },
#endif
#ifdef IPV6_MULTICAST_HOPS
	{ "IPV6_MULTICAST_HOPS", (long long)(IPV6_MULTICAST_HOPS), 0 },
#else
	{ "IPV6_MULTICAST_HOPS", 0, 1 },
#endif
#ifdef IPV6_MULTICAST_IF
	{ "IPV6_MULTICAST_IF", (long long)(IPV6_MULTICAST_IF), 0 },
#else
	{ "IPV6_MULTICAST_IF", 0, 1 },
#endif
#ifdef IPV6_MULTICAST_LOOP
	{ "IPV6_MULTICAST_LOOP", (long long)(IPV6_MULTICAST_LOOP), 0 },
#else
	{ "IPV6_MULTICAST_LOOP", 0, 1 },
#endif
#ifdef IPV6_UNICAST_HOPS
	{ "IPV6_UNICAST_HOPS", (long long)(IPV6_UNICAST_HOPS), 0 },
#else
	{ "IPV6_UNICAST_HOPS", 0, 1 },
#endif
#ifdef IPV6_V6ONLY
	{ "IPV6_V6ONLY", (long long)(IPV6_V6ONLY), 0 },
#else
	{ "IPV6_V6ONLY", 0, 1 },
#endif
};

int main(void)
{
	size_t i, j, n = sizeof v / sizeof v[0];

	pc_begin("XBD.netinet_in.h.007");
	pc_test();
	for (i = 0; i < n; i++) {
		pc_check(!v[i].missing, "the headers do not define %s", v[i].name);
		for (j = i + 1; j < n; j++)
			pc_check(v[i].missing || v[j].missing || v[i].value != v[j].value, "%s and %s are both %lld", v[i].name, v[j].name, v[i].value);
	}
	pc_pass();
}
