/*
 * Four-part dotted decimal gives the four bytes left to right.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.inet_addr.001
 */
#include "ia.h"

int main(void)
{
	static const struct row rows[] = {
		{ "1.2.3.4", { 1, 2, 3, 4 } },
		{ "127.0.0.1", { 127, 0, 0, 1 } },
		{ "192.168.100.200", { 192, 168, 100, 200 } },
		{ "10.0.200.7", { 10, 0, 200, 7 } },
	};

	pc_begin("XSH.inet_addr.001");
	pc_test();
	run_rows(rows, 4);
	pc_check(inet_addr("1.2.3.4") == htonl(0x01020304UL), "inet_addr(\"1.2.3.4\") is not htonl(0x01020304)");
	pc_pass();
}
