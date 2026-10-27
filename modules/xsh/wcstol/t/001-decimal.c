/*
 * wcstol: decimal.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcstol.001
 */
#define IC_L
#include "ic.h"
#include <string.h>

static const struct ic_case tab[] = {
	{ L"0", 10, 0, -1 },
	{ L"7", 10, 7, -1 },
	{ L"42", 10, 42, -1 },
	{ L"0042", 10, 42, -1 },
	{ L"+17", 10, 17, -1 },
	{ L"123", 0, 123, -1 },
};

int main(void)
{
	pc_begin("XSH.wcstol.001");
	pc_test();
	ic_run(tab, (int)(sizeof tab / sizeof tab[0]));
	pc_pass();
}
