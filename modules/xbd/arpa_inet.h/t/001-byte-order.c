/*
 * htonl() and htons() give the most significant octet first; ntohl() and
 *   ntohs() undo that.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.arpa_inet.h.001
 */
#include <arpa/inet.h>
#include <string.h>
#include "pc.h"


int main(void)
{
	static const unsigned char l[4] = { 0xde, 0xad, 0xbe, 0xef };
	static const unsigned char s[2] = { 0xca, 0xfe };
	unsigned char b[4];
	uint32_t n32, h32;
	uint16_t n16, h16;

	pc_begin("XBD.arpa_inet.h.001");
	pc_test();
	n32 = htonl(0x01020304u);
	memcpy(b, &n32, 4);
	pc_check(b[0] == 1 && b[1] == 2 && b[2] == 3 && b[3] == 4, "htonl(0x01020304) has the octets %u %u %u %u", b[0], b[1], b[2], b[3]);
	n16 = htons(0x0102);
	memcpy(b, &n16, 2);
	pc_check(b[0] == 1 && b[1] == 2, "htons(0x0102) has the octets %u %u", b[0], b[1]);
	memcpy(&n32, l, 4);
	h32 = ntohl(n32);
	pc_check(h32 == 0xdeadbeefu, "ntohl() of the octets de ad be ef is %#lx", (unsigned long)h32);
	memcpy(&n16, s, 2);
	h16 = ntohs(n16);
	pc_check(h16 == 0xcafe, "ntohs() of the octets ca fe is %#x", (unsigned)h16);
	pc_check(ntohl(htonl(0x12345678u)) == 0x12345678u && ntohs(htons(0xbeef)) == 0xbeef, "ntohl(htonl(x)) or ntohs(htons(x)) is not x");
	pc_pass();
}
