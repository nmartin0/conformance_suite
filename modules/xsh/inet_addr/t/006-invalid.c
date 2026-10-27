/*
 * Strings that are not in the dotted notation give (in_addr_t)-1.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.inet_addr.006
 */
#include "ia.h"

int main(void)
{
	static const char *const bad[] = { "abc", "1.2.3.4.5", "1.2.x.4", "1..2.3", "a.b.c.d" };
	size_t i;

	pc_begin("XSH.inet_addr.006");
	pc_test();
	for (i = 0; i < sizeof bad / sizeof bad[0]; i++)
		pc_check(inet_addr(bad[i]) == (in_addr_t)-1, "inet_addr(\"%s\") did not give (in_addr_t)-1", bad[i]);
	pc_pass();
}
