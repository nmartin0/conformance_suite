/*
 * Special characters are valid single-character collating symbols.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.9.5-grammar.002
 */
#include "rx.h"

static void run(int cf)
{
	rx_set("[[.^.]]", cf, "^", "a");
	rx_set("[[.[.]]", cf, "[", "a]");
	rx_set("[[...]]", cf, ".", "a");
	rx_set("[[.=.]]", cf, "=", "a");
	rx_set("[[.:.]]", cf, ":", "a");
	rx_set("[[.\\.]]", cf, "\\", "a");
	rx_set("[[.^.]a]", cf, "^a", "b");
	rx_set("[a[.^.]]", cf, "^a", "b");
	rx_set("[^[.^.]]", cf, "ab", "^");
	rx_set("[[.-.]]", cf, "-", "a.");
	rx_set("[[.].]]", cf, "]", "[a");
	rx_set("[[.$.]]", cf, "$", "a");
	rx_set("[[.*.]]", cf, "*", "a");
	rx_set("[[.(.]]", cf, "(", "a)");
}

int main(void)
{
	pc_begin("XBD.9.5-grammar.002");
	pc_test();
	run(0);
	run(REG_EXTENDED);
	pc_pass();
}
