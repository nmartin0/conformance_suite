/*
 * One list mixing characters, ranges, symbols, classes and equivalence classes.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.9.3-bre.013
 */
#include "rx.h"

static void run(int cf)
{
	rx_set("[a-c[:digit:]x[=y=]]", cf, "abc059xy", "dwz-.A");
	rx_set("[^a-c[:digit:]x[=y=]]", cf, "dwz-.A", "abc059xy");
	rx_set("[[:upper:]a-c[.z.]]", cf, "AZabcz", "dy1");
	rx_set("[[:alpha:]_-]", cf, "aZ_-", "5. ");
	rx_set("[[:digit:]-]", cf, "50-", "a_");
	rx_set("[]a-c[:digit:]-]", cf, "]abc59-", "dz.");
	rx_set("[^]a-c[:digit:]-]", cf, "dz.", "]abc59-");
	rx_set("[[:lower:][:digit:]]", cf, "az09", "AZ_");
	rx_set("[[=a=][.b.]c]", cf, "abc", "d");
}

int main(void)
{
	pc_begin("XBD.9.3-bre.013");
	pc_test();
	run(0);
	run(REG_EXTENDED);
	pc_pass();
}
