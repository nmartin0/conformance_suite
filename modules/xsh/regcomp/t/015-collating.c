/*
 * Collating symbols and equivalence classes in the POSIX locale.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.regcomp.015
 */
#include "rx.h"

static void run(int cf)
{
	rx_set("[[.a.]]", cf, "a", "bA");
	rx_set("[[.a.]b]", cf, "ab", "c");
	rx_set("[[.-.]]", cf, "-", "a.");
	rx_set("[[.].]]", cf, "]", "[a");
	rx_set("[a[.b.]c]", cf, "abc", "d");
	rx_set("[[=a=]]", cf, "a", "b");
	rx_set("[[=a=]b]", cf, "ab", "c");
}

int main(void)
{
	pc_begin("XSH.regcomp.015");
	pc_test();
	run(0);
	run(REG_EXTENDED);
	pc_pass();
}
