/*
 * The IPv6 form with a trailing dotted-decimal IPv4 part gives the six high-order pieces and the four IPv4 bytes.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.inet_ntop.005
 */
#include "ip.h"

int main(void)
{
	static const struct r6 rows[] = {
		{ "0:0:0:0:0:ffff:1.2.3.4", { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xff, 0xff, 1, 2, 3, 4 } },
		{ "1:2:3:4:5:6:1.2.3.4", { 0, 1, 0, 2, 0, 3, 0, 4, 0, 5, 0, 6, 1, 2, 3, 4 } },
		{ "0:0:0:0:0:0:192.168.100.200", { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 192, 168, 100, 200 } },
	};
	size_t i;

	pc_begin("XSH.inet_ntop.005");
	pc_test();
	for (i = 0; i < 3; i++)
		want6(&rows[i]);
	pc_pass();
}
