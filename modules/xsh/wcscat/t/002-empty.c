/*
 * wcscat() with an empty destination and with an empty source.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcscat.002
 */
#include "pc.h"
#include <wchar.h>

int main(void)
{
	wchar_t d[8], e[8], src[] = { L'x', L'y', L'z', 0 }, none[] = { 0 };
	int i;

	pc_begin("XSH.wcscat.002");
	for (i = 0; i < 8; i++)
		d[i] = e[i] = L'X';
	d[0] = 0;
	e[0] = L'a';
	e[1] = L'b';
	e[2] = 0;
	pc_test();
	(void)wcscat(d, src);
	(void)wcscat(e, none);
	pc_check(d[0] == L'x' && d[1] == L'y' && d[2] == L'z' && d[3] == 0,
	    "an empty destination plus \"xyz\" is not \"xyz\"");
	for (i = 4; i < 8; i++)
		pc_check(d[i] == L'X', "d[%d] after the result was changed", i);
	pc_check(e[0] == L'a' && e[1] == L'b' && e[2] == 0, "appending an empty string changed \"ab\"");
	for (i = 3; i < 8; i++)
		pc_check(e[i] == L'X', "e[%d] after the result was changed", i);
	pc_pass();
}
