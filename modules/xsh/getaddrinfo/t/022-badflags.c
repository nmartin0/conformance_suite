/*
 * A flags value with a bit outside the listed flags fails with EAI_BADFLAGS.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getaddrinfo.022
 */
#include "ga.h"

int main(void)
{
	int known = AI_PASSIVE | AI_CANONNAME | AI_NUMERICHOST | AI_NUMERICSERV | AI_V4MAPPED | AI_ALL | AI_ADDRCONFIG;
	int b, bit = 0, rc;

	pc_begin("XSH.getaddrinfo.022");
	for (b = 30; b >= 0 && bit == 0; b--)
		if ((known & (1 << b)) == 0)
			bit = 1 << b;
	pc_check(bit != 0, "every flag bit is taken");
	pc_test();
	rc = gai_rc("127.0.0.1", "80", NUMH | bit, AF_INET, SOCK_STREAM, 0);
	pc_check(rc == EAI_BADFLAGS, "flags with unlisted bit %#x gave %d (%s), expected EAI_BADFLAGS", (unsigned)bit, rc, gai_strerror(rc));
	pc_pass();
}
