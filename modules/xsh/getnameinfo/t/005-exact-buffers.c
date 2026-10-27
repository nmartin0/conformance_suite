/*
 * Buffers just large enough for the names and their null bytes succeed and give the same names.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getnameinfo.005
 */
#include "ni.h"

int main(void)
{
	static const unsigned char a[4] = { 192, 168, 100, 200 };
	struct sockaddr_in s = mk4(a, 65535);
	char node[64], serv[32], n2[64], s2[32];
	int rc;

	pc_begin("XSH.getnameinfo.005");
	rc = getnameinfo((struct sockaddr *)&s, sizeof s, node, sizeof node, serv, sizeof serv, NUMF);
	pc_check(rc == 0, "reference call failed: %s", gai_strerror(rc));
	pc_test();
	memset(n2, 0xAA, sizeof n2);
	memset(s2, 0xAA, sizeof s2);
	rc = getnameinfo((struct sockaddr *)&s, sizeof s, n2, (socklen_t)(strlen(node) + 1), s2, (socklen_t)(strlen(serv) + 1), NUMF);
	pc_check(rc == 0, "exact-size buffers gave %d (%s)", rc, gai_strerror(rc));
	pc_check(strcmp(n2, node) == 0 && strcmp(s2, serv) == 0, "the names differ from those of the large buffers");
	pc_pass();
}
