/*
 * With a null service pointer or a service length of zero the service is not returned but the node is.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getnameinfo.007
 */
#include "ni.h"

int main(void)
{
	static const unsigned char a[4] = { 10, 20, 30, 40 };
	struct sockaddr_in s = mk4(a, 258);
	char node[64], serv[16];
	unsigned char got[4];
	int rc;
	size_t i;

	pc_begin("XSH.getnameinfo.007");
	pc_test();
	memset(node, 0xAA, sizeof node);
	rc = getnameinfo((struct sockaddr *)&s, sizeof s, node, sizeof node, NULL, 16, NUMF);
	pc_check(rc == 0, "null service pointer gave %d (%s)", rc, gai_strerror(rc));
	pc_check(terminated(node, sizeof node) && inet_pton(AF_INET, node, got) == 1 && memcmp(got, a, 4) == 0, "node is \"%s\"", node);
	memset(node, 0xAA, sizeof node);
	memset(serv, 0xAA, sizeof serv);
	rc = getnameinfo((struct sockaddr *)&s, sizeof s, node, sizeof node, serv, 0, NUMF);
	pc_check(rc == 0, "service length 0 gave %d (%s)", rc, gai_strerror(rc));
	pc_check(terminated(node, sizeof node) && inet_pton(AF_INET, node, got) == 1 && memcmp(got, a, 4) == 0, "node is \"%s\"", node);
	for (i = 0; i < sizeof serv; i++)
		pc_check((unsigned char)serv[i] == 0xAA, "the service buffer was written although its length was 0");
	pc_pass();
}
