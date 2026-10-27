/*
 * wcstod: end pointer.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcstod.007
 */
#include "fc.h"
#include <string.h>

static const struct fc_case tab[] = {
	{ L"1.5abc", 1.5L, 3 },
	{ L"1e", 1.0L, 1 },
	{ L"1e+", 1.0L, 1 },
	{ L"1ex", 1.0L, 1 },
	{ L"0x", 0.0L, 1 },
	{ L"0xg", 0.0L, 1 },
	{ L"0x1p", 1.0L, 3 },
	{ L"0x1p+", 1.0L, 3 },
	{ L"2.5.5", 2.5L, 3 },
	{ L"  7 ", 7.0L, 3 },
};

int main(void)
{
	pc_begin("XSH.wcstod.007");
	pc_test();
	fc_run(tab, (int)(sizeof tab / sizeof tab[0]));
	pc_pass();
}
