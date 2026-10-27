/*
 * For 64 pseudo-random IPv4 addresses the text inet_ntop() gives reads back as the same address, in INET_ADDRSTRLEN bytes.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.inet_ntop.014
 */
#include "ip.h"

int main(void)
{
	unsigned long x = 4242UL;
	int i;

	pc_begin("XSH.inet_ntop.014");
	pc_test();
	for (i = 0; i < 64; i++) {
		unsigned char a[4], back[4];
		char buf[INET_ADDRSTRLEN];
		int j;

		for (j = 0; j < 4; j++)
			a[j] = (unsigned char)next_rand(&x);
		pc_check(inet_ntop(AF_INET, a, buf, sizeof buf) == buf, "inet_ntop() failed for %u.%u.%u.%u", a[0], a[1], a[2], a[3]);
		pc_check(inet_pton(AF_INET, buf, back) == 1 && memcmp(a, back, 4) == 0, "\"%s\" does not read back as %u.%u.%u.%u", buf, a[0], a[1], a[2], a[3]);
	}
	pc_pass();
}
