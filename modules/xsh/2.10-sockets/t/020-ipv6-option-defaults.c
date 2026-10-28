/*
 * The defaults and values of the multicast options at the IPPROTO_IPV6
 * level.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.10-sockets.020
 *
 * On a new AF_INET6 datagram socket IPV6_MULTICAST_HOPS is 1,
 * IPV6_MULTICAST_IF is 0 and IPV6_MULTICAST_LOOP is 1. IPV6_MULTICAST_HOPS
 * accepts 5 and reads it back, and rejects 256 with EINVAL.
 * IPV6_MULTICAST_IF set to the index of an interface reads back that index
 * (a set the system refuses for that interface is skipped).
 */
#include "pcsk6.h"
#include <net/if.h>

int main(void)
{
	int s, hops, v, k;
	unsigned int idx, loop, got;
	socklen_t l;
	struct if_nameindex *list;

	pc_begin("XSH.2.10-sockets.020");
	guard(60);
	s = sock6(SOCK_DGRAM);
	pc_test();

	hops = -7;
	l = sizeof hops;
	pc_check(getsockopt(s, IPPROTO_IPV6, IPV6_MULTICAST_HOPS, &hops, &l) == 0, "getsockopt(IPV6_MULTICAST_HOPS) failed: %s", pc_errname(errno));
	pc_check(hops == 1, "the default of IPV6_MULTICAST_HOPS is %d, expected 1", hops);
	got = 77;
	l = sizeof got;
	pc_check(getsockopt(s, IPPROTO_IPV6, IPV6_MULTICAST_IF, &got, &l) == 0, "getsockopt(IPV6_MULTICAST_IF) failed: %s", pc_errname(errno));
	pc_check(got == 0, "the default of IPV6_MULTICAST_IF is %u, expected 0", got);
	loop = 77;
	l = sizeof loop;
	pc_check(getsockopt(s, IPPROTO_IPV6, IPV6_MULTICAST_LOOP, &loop, &l) == 0, "getsockopt(IPV6_MULTICAST_LOOP) failed: %s", pc_errname(errno));
	pc_check(loop == 1, "the default of IPV6_MULTICAST_LOOP is %u, expected 1", loop);

	v = 5;
	pc_check(setsockopt(s, IPPROTO_IPV6, IPV6_MULTICAST_HOPS, &v, sizeof v) == 0, "setsockopt(IPV6_MULTICAST_HOPS, 5) failed: %s", pc_errname(errno));
	hops = -7;
	l = sizeof hops;
	pc_check(getsockopt(s, IPPROTO_IPV6, IPV6_MULTICAST_HOPS, &hops, &l) == 0 && hops == 5, "IPV6_MULTICAST_HOPS did not read back 5 (got %d)", hops);
	v = 256;
	k = setsockopt(s, IPPROTO_IPV6, IPV6_MULTICAST_HOPS, &v, sizeof v);
	pc_check(k == -1 && errno == EINVAL, "setsockopt(IPV6_MULTICAST_HOPS, 256) gave %s, expected failure with EINVAL", en(k, errno));

	list = if_nameindex();
	if (list != NULL && list[0].if_index != 0) {
		idx = list[0].if_index;
		if (setsockopt(s, IPPROTO_IPV6, IPV6_MULTICAST_IF, &idx, sizeof idx) == 0) {
			got = 77;
			l = sizeof got;
			pc_check(getsockopt(s, IPPROTO_IPV6, IPV6_MULTICAST_IF, &got, &l) == 0, "getsockopt(IPV6_MULTICAST_IF) failed: %s", pc_errname(errno));
			pc_check(got == idx, "IPV6_MULTICAST_IF set to %u read back as %u", idx, got);
		}
	}
	if (list != NULL)
		if_freenameindex(list);
	pc_pass();
}
