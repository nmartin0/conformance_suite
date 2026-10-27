/*
 * The host-to-network and network-to-host functions undo each other.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.htonl.004
 */
#include "pc.h"
#include <arpa/inet.h>
#include <inttypes.h>
#include <string.h>

int main(void)
{
	static const uint16_t s[] = { 0, 1, 0x00ff, 0x0100, 0x1234, 0x8000, 0xfffe, 0xffff };
	static const uint32_t l[] = { 0, 1, 0xff, 0x100, 0x00ff00ffUL, 0x12345678UL, 0x80000000UL, 0xfffffffeUL, 0xffffffffUL };
	size_t i;

	pc_begin("XSH.htonl.004");
	pc_test();
	for (i = 0; i < sizeof s / sizeof s[0]; i++) {
		volatile uint16_t v = s[i];

		pc_check(ntohs(htons(v)) == s[i], "ntohs(htons(%#x)) is %#x", (unsigned)s[i], (unsigned)ntohs(htons(v)));
		pc_check(htons(ntohs(v)) == s[i], "htons(ntohs(%#x)) is %#x", (unsigned)s[i], (unsigned)htons(ntohs(v)));
	}
	for (i = 0; i < sizeof l / sizeof l[0]; i++) {
		volatile uint32_t v = l[i];

		pc_check(ntohl(htonl(v)) == l[i], "ntohl(htonl(%#lx)) is %#lx", (unsigned long)l[i], (unsigned long)ntohl(htonl(v)));
		pc_check(htonl(ntohl(v)) == l[i], "htonl(ntohl(%#lx)) is %#lx", (unsigned long)l[i], (unsigned long)htonl(ntohl(v)));
	}
	pc_pass();
}
