/*
 * AF_INET6 differs from the other address families.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sys_socket.h.010
 */
#include <sys/socket.h>
#include "pc.h"


/* name, value, missing */
static const struct { const char *name; long long value; int missing; } v[] = {
#ifdef AF_INET6
	{ "AF_INET6", (long long)(AF_INET6), 0 },
#else
	{ "AF_INET6", 0, 1 },
#endif
#ifdef AF_INET
	{ "AF_INET", (long long)(AF_INET), 0 },
#else
	{ "AF_INET", 0, 1 },
#endif
#ifdef AF_UNIX
	{ "AF_UNIX", (long long)(AF_UNIX), 0 },
#else
	{ "AF_UNIX", 0, 1 },
#endif
#ifdef AF_UNSPEC
	{ "AF_UNSPEC", (long long)(AF_UNSPEC), 0 },
#else
	{ "AF_UNSPEC", 0, 1 },
#endif
};

int main(void)
{
	size_t i, j, n = sizeof v / sizeof v[0];

	pc_begin("XBD.sys_socket.h.010");
	pc_test();
	for (i = 0; i < n; i++) {
		pc_check(!v[i].missing, "the headers do not define %s", v[i].name);
		for (j = i + 1; j < n; j++)
			pc_check(v[i].missing || v[j].missing || v[i].value != v[j].value, "%s and %s are both %lld", v[i].name, v[j].name, v[i].value);
	}
	pc_pass();
}
