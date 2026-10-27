/*
 * Leading zeros in the IPv4 parts are allowed and the parts stay decimal.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.inet_ntop.002
 */
#include "ip.h"

int main(void)
{
	static const struct r4 rows[] = {
		{ "001.002.003.004", { 1, 2, 3, 4 } },
		{ "010.0.0.1", { 10, 0, 0, 1 } },
		{ "000.000.000.000", { 0, 0, 0, 0 } },
		{ "192.168.001.099", { 192, 168, 1, 99 } },
		{ "255.255.255.0", { 255, 255, 255, 0 } },
	};
	size_t i;

	pc_begin("XSH.inet_ntop.002");
	pc_test();
	for (i = 0; i < 5; i++)
		want4(&rows[i]);
	pc_pass();
}
