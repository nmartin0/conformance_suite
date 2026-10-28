/*
 * Every non-special byte is an ordinary character that matches only itself.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.9.1-definitions.001
 */
#include "rx.h"

int main(void)
{
	static const char bre_special[] = ".[\\*^$";
	static const char ere_special[] = "^.[$()|*+?{\\";
	char pat[2];
	int c;

	pc_begin("XBD.9.1-definitions.001");
	pc_test();
	pat[1] = '\0';
	for (c = 1; c <= 255; c++) {
		pat[0] = (char)c;
		if (strchr(bre_special, c) == NULL)
			rx_hay(pat, 0, c);
		if (strchr(ere_special, c) == NULL)
			rx_hay(pat, REG_EXTENDED, c);
	}
	pc_pass();
}
