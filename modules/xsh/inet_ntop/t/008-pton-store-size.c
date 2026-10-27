/*
 * inet_pton() stores only four bytes for AF_INET and sixteen for AF_INET6.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.inet_ntop.008
 */
#include "ip.h"

int main(void)
{
	struct { unsigned char d[4]; unsigned char g[8]; } a;
	struct { unsigned char d[16]; unsigned char g[8]; } b;
	size_t i;

	pc_begin("XSH.inet_ntop.008");
	memset(&a, 0xA5, sizeof a);
	memset(&b, 0xA5, sizeof b);
	pc_test();
	pc_check(inet_pton(AF_INET, "1.2.3.4", a.d) == 1, "AF_INET conversion failed");
	pc_check(inet_pton(AF_INET6, "1:2:3:4:5:6:7:8", b.d) == 1, "AF_INET6 conversion failed");
	for (i = 0; i < 8; i++) {
		pc_check(a.g[i] == 0xA5, "AF_INET wrote beyond four bytes");
		pc_check(b.g[i] == 0xA5, "AF_INET6 wrote beyond sixteen bytes");
	}
	pc_pass();
}
