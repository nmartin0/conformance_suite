/*
 * inet_ntop() of IPv6 addresses returns dst holding text that reads back as the address, within INET6_ADDRSTRLEN.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.inet_ntop.011
 */
#include "ip.h"

int main(void)
{
	static const struct r6 rows[] = {
		{ "::", { 0 } },
		{ "::1", { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 } },
		{ "1::", { 0, 1, 0 } },
		{ "1:2:3:4:5:6:7:8", { 0, 1, 0, 2, 0, 3, 0, 4, 0, 5, 0, 6, 0, 7, 0, 8 } },
		{ "2001:db8::ff00:42:8329", { 0x20, 0x01, 0x0d, 0xb8, 0, 0, 0, 0, 0, 0, 0xff, 0x00, 0x00, 0x42, 0x83, 0x29 } },
		{ "all ones", { 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 } },
		{ "mapped", { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xff, 0xff, 255, 255, 255, 255 } },
	};
	size_t i;

	pc_begin("XSH.inet_ntop.011");
	pc_test();
	for (i = 0; i < sizeof rows / sizeof rows[0]; i++) {
		char buf[INET6_ADDRSTRLEN];
		unsigned char back[16];
		const char *r = inet_ntop(AF_INET6, rows[i].b, buf, sizeof buf);

		pc_check(r == buf, "inet_ntop() did not return dst for %s", rows[i].text);
		pc_check(inet_pton(AF_INET6, buf, back) == 1 && memcmp(back, rows[i].b, 16) == 0, "\"%s\" does not read back as the address (%s)", buf, rows[i].text);
	}
	pc_pass();
}
