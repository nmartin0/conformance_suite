/*
 * Parts written in octal or hexadecimal, as C writes them, are accepted.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.inet_addr.005
 */
#include "ia.h"

int main(void)
{
	static const struct row rows[] = {
		{ "0x7f.0.0.1", { 127, 0, 0, 1 } },
		{ "0X7F.0.0.0X1", { 127, 0, 0, 1 } },
		{ "0177.0.0.01", { 127, 0, 0, 1 } },
		{ "0x0a.010.8.0x10", { 10, 8, 8, 16 } },
		{ "10.0x10000", { 10, 1, 0, 0 } },
		{ "0300.0250.0144.0310", { 192, 168, 100, 200 } },
	};

	pc_begin("XSH.inet_addr.005");
	pc_test();
	run_rows(rows, 6);
	pc_pass();
}
