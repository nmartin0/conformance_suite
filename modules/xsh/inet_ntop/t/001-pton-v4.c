/*
 * inet_pton() reads dotted-decimal IPv4 text into four bytes, first part first.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.inet_ntop.001
 */
#include "ip.h"

int main(void)
{
	static const struct r4 rows[] = {
		{ "0.0.0.0", { 0, 0, 0, 0 } },
		{ "1.2.3.4", { 1, 2, 3, 4 } },
		{ "127.0.0.1", { 127, 0, 0, 1 } },
		{ "192.168.100.200", { 192, 168, 100, 200 } },
		{ "255.255.255.255", { 255, 255, 255, 255 } },
	};
	size_t i;

	pc_begin("XSH.inet_ntop.001");
	pc_test();
	for (i = 0; i < 5; i++)
		want4(&rows[i]);
	pc_pass();
}
