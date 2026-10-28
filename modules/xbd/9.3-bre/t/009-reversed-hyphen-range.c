/*
 * A reversed range ending in hyphen-minus is invalid or equals the list @.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.9.3-bre.009
 */
#include "rx.h"

static void run(int cf)
{
	regex_t re;
	int r = regcomp(&re, "[a--@]", cf);

	if (r != 0) {
		pc_note("flags 0x%x: rejected with %d", (unsigned)cf, r);
		return;
	}
	regfree(&re);
	pc_note("flags 0x%x: accepted", (unsigned)cf);
	rx_set("[a--@]", cf, "@", "a-b,`A");
}

int main(void)
{
	pc_begin("XBD.9.3-bre.009");
	pc_test();
	run(0);
	run(REG_EXTENDED);
	pc_pass();
}
