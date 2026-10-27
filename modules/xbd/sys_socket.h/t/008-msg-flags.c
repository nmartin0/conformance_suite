/*
 * The MSG_ flags have distinct values.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sys_socket.h.008
 */
#include <sys/socket.h>
#include "pc.h"


/* name, value, missing */
static const struct { const char *name; long long value; int missing; } v[] = {
#ifdef MSG_CMSG_CLOEXEC
	{ "MSG_CMSG_CLOEXEC", (long long)(MSG_CMSG_CLOEXEC), 0 },
#else
	{ "MSG_CMSG_CLOEXEC", 0, 1 },
#endif
#ifdef MSG_CMSG_CLOFORK
	{ "MSG_CMSG_CLOFORK", (long long)(MSG_CMSG_CLOFORK), 0 },
#else
	{ "MSG_CMSG_CLOFORK", 0, 1 },
#endif
#ifdef MSG_CTRUNC
	{ "MSG_CTRUNC", (long long)(MSG_CTRUNC), 0 },
#else
	{ "MSG_CTRUNC", 0, 1 },
#endif
#ifdef MSG_DONTROUTE
	{ "MSG_DONTROUTE", (long long)(MSG_DONTROUTE), 0 },
#else
	{ "MSG_DONTROUTE", 0, 1 },
#endif
#ifdef MSG_EOR
	{ "MSG_EOR", (long long)(MSG_EOR), 0 },
#else
	{ "MSG_EOR", 0, 1 },
#endif
#ifdef MSG_OOB
	{ "MSG_OOB", (long long)(MSG_OOB), 0 },
#else
	{ "MSG_OOB", 0, 1 },
#endif
#ifdef MSG_NOSIGNAL
	{ "MSG_NOSIGNAL", (long long)(MSG_NOSIGNAL), 0 },
#else
	{ "MSG_NOSIGNAL", 0, 1 },
#endif
#ifdef MSG_PEEK
	{ "MSG_PEEK", (long long)(MSG_PEEK), 0 },
#else
	{ "MSG_PEEK", 0, 1 },
#endif
#ifdef MSG_TRUNC
	{ "MSG_TRUNC", (long long)(MSG_TRUNC), 0 },
#else
	{ "MSG_TRUNC", 0, 1 },
#endif
#ifdef MSG_WAITALL
	{ "MSG_WAITALL", (long long)(MSG_WAITALL), 0 },
#else
	{ "MSG_WAITALL", 0, 1 },
#endif
};

int main(void)
{
	size_t i, j, n = sizeof v / sizeof v[0];

	pc_begin("XBD.sys_socket.h.008");
	pc_test();
	for (i = 0; i < n; i++) {
		pc_check(!v[i].missing, "the headers do not define %s", v[i].name);
		for (j = i + 1; j < n; j++)
			pc_check(v[i].missing || v[j].missing || v[i].value != v[j].value, "%s and %s are both %lld", v[i].name, v[j].name, v[i].value);
	}
	pc_pass();
}
