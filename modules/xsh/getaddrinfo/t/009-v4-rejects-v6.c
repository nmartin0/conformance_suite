/*
 * With AF_INET an IPv6 address string yields no result.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getaddrinfo.009
 */
#include "ga.h"

int main(void)
{
	int rc;

	pc_begin("XSH.getaddrinfo.009");
	pc_test();
	rc = gai_rc("::1", "80", NUMH, AF_INET, SOCK_STREAM, 0);
	pc_check(rc != 0, "AF_INET with an IPv6 address string succeeded");
	/*
	 * The failure is an error value that applies: the flags, the service,
	 * the socket type and the memory are all fine here, and the error
	 * has a text (gai_strerror() is the interface that gives it).
	 */
	pc_check(rc != EAI_BADFLAGS && rc != EAI_SERVICE && rc != EAI_SOCKTYPE && rc != EAI_MEMORY && rc != EAI_OVERFLOW,
	    "AF_INET with an IPv6 address string gave %d, an error value for a different cause", rc);
	{
		const char *msg = gai_strerror(rc);

		pc_check(msg != NULL && msg[0] != '\0', "gai_strerror(%d) gave no text", rc);
	}
	pc_pass();
}
