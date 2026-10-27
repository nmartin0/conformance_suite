/*
 * inet_ntop() of an IPv4 address returns dst holding dotted-decimal text that reads back as the address.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.inet_ntop.009
 */
#include "ip.h"

int main(void)
{
	static const struct r4 exact[] = {
		{ "192.168.100.200", { 192, 168, 100, 200 } },
		{ "255.255.255.255", { 255, 255, 255, 255 } },
	};
	static const unsigned char others[][4] = { { 0, 0, 0, 0 }, { 1, 2, 3, 4 }, { 127, 0, 0, 1 }, { 10, 0, 200, 7 } };
	size_t i;

	pc_begin("XSH.inet_ntop.009");
	pc_test();
	for (i = 0; i < 2; i++) {
		char buf[INET_ADDRSTRLEN];
		const char *r = inet_ntop(AF_INET, exact[i].b, buf, sizeof buf);

		pc_check(r == buf, "inet_ntop() did not return dst for %s", exact[i].text);
		pc_check(strcmp(buf, exact[i].text) == 0, "inet_ntop() gave \"%s\", expected \"%s\"", buf, exact[i].text);
	}
	for (i = 0; i < 4; i++) {
		char buf[INET_ADDRSTRLEN];
		unsigned char back[4];
		const char *r = inet_ntop(AF_INET, others[i], buf, sizeof buf);

		pc_check(r == buf, "inet_ntop() did not return dst");
		pc_check(inet_pton(AF_INET, buf, back) == 1 && memcmp(back, others[i], 4) == 0, "\"%s\" does not read back as the address", buf);
	}
	pc_pass();
}
