/*
 * wcstoul: digit limits.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcstoul.004
 */
#define IC_UL
#include "ic.h"
#include <string.h>

static const struct ic_case tab[] = {
	{ L"1012", 2, 5, 3 },
	{ L"789", 8, 7, 1 },
	{ L"1fg", 16, 31, 2 },
	{ L"12ab", 10, 12, 2 },
	{ L"0x", 16, 0, 1 },
};

int main(void)
{
	pc_begin("XSH.wcstoul.004");
	pc_test();
	ic_run(tab, (int)(sizeof tab / sizeof tab[0]));
	pc_pass();
}
