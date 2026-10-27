/*
 * wcstoimax: base zero.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcstoimax.003
 */
#define IC_IMAX
#include "ic.h"
#include <string.h>

static const struct ic_case tab[] = {
	{ L"0x1F", 0, 31, -1 },
	{ L"0X1f", 0, 31, -1 },
	{ L"017", 0, 15, -1 },
	{ L"0", 0, 0, -1 },
	{ L"10", 0, 10, -1 },
	{ L"08", 0, 0, 1 },
	{ L"0x", 0, 0, 1 },
	{ L"0xg", 0, 0, 1 },
};

int main(void)
{
	pc_begin("XSH.wcstoimax.003");
	pc_test();
	ic_run(tab, (int)(sizeof tab / sizeof tab[0]));
	pc_pass();
}
