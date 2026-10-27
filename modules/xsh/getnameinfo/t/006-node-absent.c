/*
 * With a null node pointer or a node length of zero the node name is not returned but the service is.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getnameinfo.006
 */
#include "ni.h"

int main(void)
{
	static const unsigned char a[4] = { 10, 20, 30, 40 };
	struct sockaddr_in s = mk4(a, 258);
	char node[16], serv[32];
	int rc, ok;

	pc_begin("XSH.getnameinfo.006");
	pc_test();
	memset(serv, 0xAA, sizeof serv);
	rc = getnameinfo((struct sockaddr *)&s, sizeof s, NULL, 16, serv, sizeof serv, NUMF);
	pc_check(rc == 0, "null node pointer gave %d (%s)", rc, gai_strerror(rc));
	pc_check(terminated(serv, sizeof serv) && digits_value(serv, &ok) == 258 && ok, "service is \"%s\"", serv);
	memset(node, 0xAA, sizeof node);
	memset(serv, 0xAA, sizeof serv);
	rc = getnameinfo((struct sockaddr *)&s, sizeof s, node, 0, serv, sizeof serv, NUMF);
	pc_check(rc == 0, "node length 0 gave %d (%s)", rc, gai_strerror(rc));
	pc_check(terminated(serv, sizeof serv) && digits_value(serv, &ok) == 258 && ok, "service is \"%s\"", serv);
	{
		size_t i;

		for (i = 0; i < sizeof node; i++)
			pc_check((unsigned char)node[i] == 0xAA, "the node buffer was written although its length was 0");
	}
	pc_pass();
}
