/*
 * SHUT_RD, SHUT_RDWR and SHUT_WR have distinct values.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sys_socket.h.011
 */
#include <sys/socket.h>
#include "pc.h"


/* name, value, missing */
static const struct { const char *name; long long value; int missing; } v[] = {
#ifdef SHUT_RD
	{ "SHUT_RD", (long long)(SHUT_RD), 0 },
#else
	{ "SHUT_RD", 0, 1 },
#endif
#ifdef SHUT_RDWR
	{ "SHUT_RDWR", (long long)(SHUT_RDWR), 0 },
#else
	{ "SHUT_RDWR", 0, 1 },
#endif
#ifdef SHUT_WR
	{ "SHUT_WR", (long long)(SHUT_WR), 0 },
#else
	{ "SHUT_WR", 0, 1 },
#endif
};

int main(void)
{
	size_t i, j, n = sizeof v / sizeof v[0];

	pc_begin("XBD.sys_socket.h.011");
	pc_test();
	for (i = 0; i < n; i++) {
		pc_check(!v[i].missing, "the headers do not define %s", v[i].name);
		for (j = i + 1; j < n; j++)
			pc_check(v[i].missing || v[j].missing || v[i].value != v[j].value, "%s and %s are both %lld", v[i].name, v[j].name, v[i].value);
	}
	pc_pass();
}
