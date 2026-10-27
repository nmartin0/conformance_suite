/*
 * inet_pton() reads the standard IPv6 text forms into sixteen bytes.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.inet_ntop.004
 */
#include "ip.h"

int main(void)
{
	static const struct r6 rows[] = {
		{ "::", { 0 } },
		{ "0:0:0:0:0:0:0:0", { 0 } },
		{ "::1", { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 } },
		{ "0:0:0:0:0:0:0:1", { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 } },
		{ "1::", { 0, 1, 0 } },
		{ "1:2:3:4:5:6:7:8", { 0, 1, 0, 2, 0, 3, 0, 4, 0, 5, 0, 6, 0, 7, 0, 8 } },
		{ "0001:0002:0003:0004:0005:0006:0007:0008", { 0, 1, 0, 2, 0, 3, 0, 4, 0, 5, 0, 6, 0, 7, 0, 8 } },
		{ "1::8", { 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8 } },
		{ "2001:db8::ff00:42:8329", { 0x20, 0x01, 0x0d, 0xb8, 0, 0, 0, 0, 0, 0, 0xff, 0x00, 0x00, 0x42, 0x83, 0x29 } },
		{ "FFFF:ffff:AbCd:0:0:0:0:1", { 0xff, 0xff, 0xff, 0xff, 0xab, 0xcd, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 } },
		{ "ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff", { 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 } },
	};
	size_t i;

	pc_begin("XSH.inet_ntop.004");
	pc_test();
	for (i = 0; i < sizeof rows / sizeof rows[0]; i++)
		want6(&rows[i]);
	pc_pass();
}
