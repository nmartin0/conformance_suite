/*
 * wcstod: hex.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcstod.002
 */
#include "fc.h"
#include <string.h>

static const struct fc_case tab[] = {
	{ L"0x1p4", 16.0L, -1 },
	{ L"0X1P4", 16.0L, -1 },
	{ L"0x1.8p1", 3.0L, -1 },
	{ L"0x.8", 0.5L, -1 },
	{ L"0xA.8p-1", 5.25L, -1 },
	{ L"0x10", 16.0L, -1 },
	{ L"0x1p+1", 2.0L, -1 },
};

int main(void)
{
	pc_begin("XSH.wcstod.002");
	pc_test();
	fc_run(tab, (int)(sizeof tab / sizeof tab[0]));
	pc_pass();
}
