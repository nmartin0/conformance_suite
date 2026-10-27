/*
 * wcstol: bases.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcstol.002
 */
#define IC_L
#include "ic.h"
#include <string.h>

static const struct ic_case tab[] = {
	{ L"1011", 2, 11, -1 },
	{ L"777", 8, 511, -1 },
	{ L"ff", 16, 255, -1 },
	{ L"FF", 16, 255, -1 },
	{ L"0xff", 16, 255, -1 },
	{ L"0XfF", 16, 255, -1 },
	{ L"zz", 36, 1295, -1 },
	{ L"ZZ", 36, 1295, -1 },
	{ L"b9", 12, 141, -1 },
};

int main(void)
{
	pc_begin("XSH.wcstol.002");
	pc_test();
	ic_run(tab, (int)(sizeof tab / sizeof tab[0]));
	pc_pass();
}
