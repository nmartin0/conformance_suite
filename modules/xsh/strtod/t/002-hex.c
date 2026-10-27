/*
 * strtod: hex.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strtod.002
 */
#include "fc.h"
#include <string.h>

static const struct fc_case tab[] = {
	{ "0x1p4", 16.0L, -1 },
	{ "0X1P4", 16.0L, -1 },
	{ "0x1.8p1", 3.0L, -1 },
	{ "0x.8", 0.5L, -1 },
	{ "0xA.8p-1", 5.25L, -1 },
	{ "0x10", 16.0L, -1 },
	{ "0x1p+1", 2.0L, -1 },
};

int main(void)
{
	pc_begin("XSH.strtod.002");
	pc_test();
	fc_run(tab, (int)(sizeof tab / sizeof tab[0]));
	pc_pass();
}
