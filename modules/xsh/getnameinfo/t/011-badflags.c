/*
 * A flags value with a bit outside the listed flags fails with EAI_BADFLAGS.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getnameinfo.011
 */
#include "ni.h"

int main(void)
{
	static const unsigned char a[4] = { 127, 0, 0, 1 };
	struct sockaddr_in s = mk4(a, 80);
	char node[64], serv[32];
	int known = NI_NOFQDN | NI_NUMERICHOST | NI_NAMEREQD | NI_NUMERICSERV | NI_NUMERICSCOPE | NI_DGRAM;
	int b, bit = 0, rc;

	pc_begin("XSH.getnameinfo.011");
	for (b = 30; b >= 0 && bit == 0; b--)
		if ((known & (1 << b)) == 0)
			bit = 1 << b;
	pc_check(bit != 0, "every flag bit is taken");
	pc_test();
	rc = getnameinfo((struct sockaddr *)&s, sizeof s, node, sizeof node, serv, sizeof serv, NUMF | bit);
	pc_check(rc == EAI_BADFLAGS, "flags with unlisted bit %#x gave %d (%s), expected EAI_BADFLAGS", (unsigned)bit, rc, gai_strerror(rc));
	pc_pass();
}
