/*
 * Bracket expression structure: closing bracket, left bracket, circumflex.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.9.3-bre.007
 */
#include "rx.h"

static void run(int cf)
{
	rx_one("[a]b]", cf, "ab]", 0, 3);
	rx_one("[a]b]", cf, "bb]", -1, 0);
	rx_one("[a]b]", cf, "a]", -1, 0);
	rx_one("x[]]y", cf, "ax]y", 1, 4);
	rx_set("[]]", cf, "]", "a[");
	rx_set("[^]]", cf, "a[", "]");
	rx_set("[]-]", cf, "]-", "a.0A\\[");
	rx_set("[^]-]", cf, "a.0A\\[", "]-");
	rx_set("[^^]", cf, "a]-", "^");
	rx_set("[a[b]", cf, "a[b", "c]");
	rx_set("[[a]", cf, "[a", "b]");
	rx_set("[a[]", cf, "a[", "b]");
	rx_set("[a[.].]b]", cf, "a]b", "c[");
}

int main(void)
{
	pc_begin("XBD.9.3-bre.007");
	pc_test();
	run(0);
	run(REG_EXTENDED);
	pc_pass();
}
