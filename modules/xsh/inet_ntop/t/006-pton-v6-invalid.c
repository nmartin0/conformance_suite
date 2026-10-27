/*
 * IPv6 text that is not one of the standard forms gives 0.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.inet_ntop.006
 */
#include "ip.h"

int main(void)
{
	static const char *const bad[] = {
		"", ":", ":::", "1:2:3:4:5:6:7", "1:2:3:4:5:6:7:8:9", "1::2::3", "12345::1", "g::1", "::g",
		":1:2:3:4:5:6:7", "1:2:3:4:5:6:7:", "1:2:3:4:5:6:1.2.3", "::1.2.3.256", "1.2.3.4", "1:2:3:4:5:6:7:8:",
	};
	size_t i;

	pc_begin("XSH.inet_ntop.006");
	pc_test();
	for (i = 0; i < sizeof bad / sizeof bad[0]; i++)
		reject(AF_INET6, bad[i]);
	pc_pass();
}
