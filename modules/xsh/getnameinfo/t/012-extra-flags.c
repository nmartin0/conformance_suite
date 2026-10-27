/*
 * NI_NOFQDN, NI_NUMERICSCOPE and NI_DGRAM are accepted alongside the numeric flags and leave the numeric results unchanged.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getnameinfo.012
 */
#include "ni.h"

int main(void)
{
	static const int extra[] = { NI_NOFQDN, NI_NUMERICSCOPE, NI_DGRAM, NI_NOFQDN | NI_NUMERICSCOPE | NI_DGRAM };
	static const unsigned char a[4] = { 10, 20, 30, 40 };
	struct sockaddr_in s = mk4(a, 258);
	char node[64], serv[32], n2[64], s2[32];
	size_t i;
	int rc;

	pc_begin("XSH.getnameinfo.012");
	rc = getnameinfo((struct sockaddr *)&s, sizeof s, node, sizeof node, serv, sizeof serv, NUMF);
	pc_check(rc == 0, "reference call failed: %s", gai_strerror(rc));
	pc_test();
	for (i = 0; i < 4; i++) {
		rc = getnameinfo((struct sockaddr *)&s, sizeof s, n2, sizeof n2, s2, sizeof s2, NUMF | extra[i]);
		pc_check(rc == 0, "flags %#x gave %d (%s)", (unsigned)extra[i], rc, gai_strerror(rc));
		pc_check(strcmp(n2, node) == 0 && strcmp(s2, serv) == 0, "flags %#x changed the result to \"%s\", \"%s\"", (unsigned)extra[i], n2, s2);
	}
	pc_pass();
}
