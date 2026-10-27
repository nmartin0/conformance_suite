/*
 * A node buffer that cannot hold the name and its null byte gives EAI_OVERFLOW.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getnameinfo.003
 */
#include "ni.h"

int main(void)
{
	static const unsigned char a[4] = { 192, 168, 100, 200 };
	struct sockaddr_in s = mk4(a, 80);
	char node[64], serv[32];
	size_t n;
	int rc;

	pc_begin("XSH.getnameinfo.003");
	rc = getnameinfo((struct sockaddr *)&s, sizeof s, node, sizeof node, serv, sizeof serv, NUMF);
	pc_check(rc == 0, "reference call failed: %s", gai_strerror(rc));
	n = strlen(node);
	pc_test();
	rc = getnameinfo((struct sockaddr *)&s, sizeof s, node, (socklen_t)n, serv, sizeof serv, NUMF);
	pc_check(rc == EAI_OVERFLOW, "a node buffer of %lu bytes for a %lu-character name gave %d (%s)", (unsigned long)n, (unsigned long)n, rc, gai_strerror(rc));
	rc = getnameinfo((struct sockaddr *)&s, sizeof s, node, 1, serv, sizeof serv, NUMF);
	pc_check(rc == EAI_OVERFLOW, "a node buffer of 1 byte gave %d (%s)", rc, gai_strerror(rc));
	pc_pass();
}
