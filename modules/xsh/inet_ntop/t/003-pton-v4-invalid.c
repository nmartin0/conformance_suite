/*
 * IPv4 text that is not four decimal parts of at most 255 gives 0.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.inet_ntop.003
 */
#include "ip.h"

int main(void)
{
	static const char *const bad[] = {
		"256.1.1.1", "1.256.1.1", "1.2.3.256", "999.1.1.1", "1.2.3", "1.2", "1", "16909060", "1.2.3.4.5",
		"0x7f.0.0.1", "1.2.3.0x4", "1.2.3.", ".1.2.3", "1..2.3", "abc", "", "1.2.3.4a", "1.2.3.-4", "::1",
	};
	size_t i;

	pc_begin("XSH.inet_ntop.003");
	pc_test();
	for (i = 0; i < sizeof bad / sizeof bad[0]; i++)
		reject(AF_INET, bad[i]);
	pc_pass();
}
