/*
 * Three-part form puts the last part, as 16 bits, in the two rightmost bytes.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.inet_addr.002
 */
#include "ia.h"

int main(void)
{
	static const struct row rows[] = {
		{ "10.20.258", { 10, 20, 1, 2 } },
		{ "128.5.65535", { 128, 5, 255, 255 } },
		{ "1.2.0", { 1, 2, 0, 0 } },
	};

	pc_begin("XSH.inet_addr.002");
	pc_test();
	run_rows(rows, 3);
	pc_pass();
}
