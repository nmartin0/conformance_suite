/*
 * inet_addr() of the text inet_ntoa() gives returns the original address.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.inet_addr.009
 */
#include "ia.h"

int main(void)
{
	unsigned long x = 12345UL;
	int i;

	pc_begin("XSH.inet_addr.009");
	pc_test();
	for (i = 0; i < 64; i++) {
		struct in_addr in;
		unsigned char b[4];
		char *s;
		int j;

		for (j = 0; j < 4; j++) {
			x = x * 1103515245UL + 12345UL;
			b[j] = (unsigned char)(x >> 16);
		}
		if (b[0] == 255 && b[1] == 255 && b[2] == 255 && b[3] == 255)
			continue;
		memcpy(&in.s_addr, b, 4);
		s = inet_ntoa(in);
		pc_check(s != NULL, "inet_ntoa() returned a null pointer");
		{
			char text[32];
			unsigned char got[4];

			snprintf(text, sizeof text, "%s", s);
			bytes_of(inet_addr(text), got);
			pc_check(memcmp(got, b, 4) == 0, "inet_addr(\"%s\") is not the address inet_ntoa() was given", text);
		}
	}
	pc_pass();
}
