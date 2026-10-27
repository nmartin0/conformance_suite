/*
 * For 64 pseudo-random IPv6 addresses, many with zero pieces, the text inet_ntop() gives reads back as the same address, in INET6_ADDRSTRLEN bytes.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.inet_ntop.015
 */
#include "ip.h"

int main(void)
{
	unsigned long x = 99UL;
	int i;

	pc_begin("XSH.inet_ntop.015");
	pc_test();
	for (i = 0; i < 64; i++) {
		unsigned char a[16], back[16];
		char buf[INET6_ADDRSTRLEN];
		int j;

		for (j = 0; j < 16; j += 2) {
			unsigned long r = next_rand(&x);

			if (r % 3 == 0) {
				a[j] = 0;
				a[j + 1] = 0;
			} else {
				a[j] = (unsigned char)(r >> 8);
				a[j + 1] = (unsigned char)r;
			}
		}
		pc_check(inet_ntop(AF_INET6, a, buf, sizeof buf) == buf, "inet_ntop() failed for address %d", i);
		pc_check(inet_pton(AF_INET6, buf, back) == 1 && memcmp(a, back, 16) == 0, "\"%s\" does not read back as address %d", buf, i);
	}
	pc_pass();
}
