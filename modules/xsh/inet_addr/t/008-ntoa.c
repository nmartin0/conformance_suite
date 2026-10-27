/*
 * inet_ntoa() writes the address as dotted decimal: bytes left to right.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.inet_addr.008
 */
#include "ia.h"

int main(void)
{
	static const struct row rows[] = {
		{ "1.2.3.4", { 1, 2, 3, 4 } },
		{ "0.0.0.0", { 0, 0, 0, 0 } },
		{ "255.255.255.255", { 255, 255, 255, 255 } },
		{ "10.0.200.7", { 10, 0, 200, 7 } },
		{ "192.168.100.200", { 192, 168, 100, 200 } },
	};
	int i;

	pc_begin("XSH.inet_addr.008");
	pc_test();
	for (i = 0; i < 5; i++) {
		struct in_addr in;
		char *s;

		memcpy(&in.s_addr, rows[i].b, 4);
		s = inet_ntoa(in);
		pc_check(s != NULL, "inet_ntoa() returned a null pointer");
		pc_check(strcmp(s, rows[i].text) == 0, "inet_ntoa() gave \"%s\", expected \"%s\"", s, rows[i].text);
	}
	pc_pass();
}
