/*
 * A single number gives the address whose bytes, left to right, are those of the number from its most significant byte.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.inet_addr.004
 */
#include "ia.h"

int main(void)
{
	static const struct row rows[] = {
		{ "16909060", { 1, 2, 3, 4 } },
		{ "0x7f000001", { 127, 0, 0, 1 } },
		{ "255", { 0, 0, 0, 255 } },
	};

	pc_begin("XSH.inet_addr.004");
	pc_test();
	run_rows(rows, 3);
	pc_pass();
}
