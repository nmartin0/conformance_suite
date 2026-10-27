/*
 * A service name that no database entry has fails with EAI_SERVICE or EAI_NONAME.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getaddrinfo.029
 */
#include "ga.h"

int main(void)
{
	int rc;

	pc_begin("XSH.getaddrinfo.029");
	pc_test();
	rc = gai_rc("127.0.0.1", "pc-no-such-service-zz", AI_NUMERICHOST, AF_INET, SOCK_STREAM, 0);
	pc_check(rc == EAI_SERVICE || rc == EAI_NONAME, "an unknown service name gave %d (%s)", rc, gai_strerror(rc));
	pc_pass();
}
