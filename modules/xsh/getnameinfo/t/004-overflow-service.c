/*
 * A service buffer that cannot hold the service text and its null byte gives EAI_OVERFLOW.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getnameinfo.004
 */
#include "ni.h"

int main(void)
{
	static const unsigned char a[4] = { 192, 168, 100, 200 };
	struct sockaddr_in s = mk4(a, 65535);
	char node[64], serv[32];
	size_t n;
	int rc;

	pc_begin("XSH.getnameinfo.004");
	rc = getnameinfo((struct sockaddr *)&s, sizeof s, node, sizeof node, serv, sizeof serv, NUMF);
	pc_check(rc == 0, "reference call failed: %s", gai_strerror(rc));
	n = strlen(serv);
	pc_test();
	rc = getnameinfo((struct sockaddr *)&s, sizeof s, node, sizeof node, serv, (socklen_t)n, NUMF);
	pc_check(rc == EAI_OVERFLOW, "a service buffer of %lu bytes for a %lu-character text gave %d (%s)", (unsigned long)n, (unsigned long)n, rc, gai_strerror(rc));
	rc = getnameinfo((struct sockaddr *)&s, sizeof s, node, sizeof node, serv, 1, NUMF);
	pc_check(rc == EAI_OVERFLOW, "a service buffer of 1 byte gave %d (%s)", rc, gai_strerror(rc));
	pc_pass();
}
