/*
 * Two-part form puts the last part, as 24 bits, in the three rightmost bytes.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.inet_addr.003
 */
#include "ia.h"

int main(void)
{
	static const struct row rows[] = {
		{ "10.65793", { 10, 1, 1, 1 } },
		{ "127.1", { 127, 0, 0, 1 } },
		{ "5.16777215", { 5, 255, 255, 255 } },
	};

	pc_begin("XSH.inet_addr.003");
	pc_test();
	run_rows(rows, 3);
	pc_pass();
}
