/*
 * Collating symbols for ordinary printable characters.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.9.3-bre.011
 */
#include "rx.h"

static void run(int cf)
{
	char pat[16];
	int c;

	for (c = 0x20; c <= 0x7e; c++) {
		if (strchr(".[]^-\\", c) != NULL)
			continue;
		snprintf(pat, sizeof pat, "[[.%c.]]", c);
		rx_hay(pat, cf, c);
	}
}

int main(void)
{
	pc_begin("XBD.9.3-bre.011");
	pc_test();
	run(0);
	run(REG_EXTENDED);
	pc_pass();
}
