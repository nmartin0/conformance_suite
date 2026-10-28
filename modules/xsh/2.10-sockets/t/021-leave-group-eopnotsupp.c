/*
 * IPV6_LEAVE_GROUP cannot be read with getsockopt().
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.10-sockets.021
 */
#include "pcsk6.h"

int main(void)
{
	struct ipv6_mreq mr;
	socklen_t l = sizeof mr;
	int s, r;

	pc_begin("XSH.2.10-sockets.021");
	guard(60);
	s = sock6(SOCK_DGRAM);
	memset(&mr, 0, sizeof mr);
	pc_test();
	r = getsockopt(s, IPPROTO_IPV6, IPV6_LEAVE_GROUP, &mr, &l);
	pc_check(r == -1 && errno == EOPNOTSUPP, "getsockopt(IPV6_LEAVE_GROUP) gave %s, expected failure with EOPNOTSUPP", en(r, errno));
	pc_pass();
}
