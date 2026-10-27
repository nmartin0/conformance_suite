/*
 * SOCK_DGRAM, SOCK_SEQPACKET and SOCK_STREAM have distinct values.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sys_socket.h.004
 */
#include <sys/socket.h>
#include "pc.h"


/* name, value, missing */
static const struct { const char *name; long long value; int missing; } v[] = {
#ifdef SOCK_DGRAM
	{ "SOCK_DGRAM", (long long)(SOCK_DGRAM), 0 },
#else
	{ "SOCK_DGRAM", 0, 1 },
#endif
#ifdef SOCK_SEQPACKET
	{ "SOCK_SEQPACKET", (long long)(SOCK_SEQPACKET), 0 },
#else
	{ "SOCK_SEQPACKET", 0, 1 },
#endif
#ifdef SOCK_STREAM
	{ "SOCK_STREAM", (long long)(SOCK_STREAM), 0 },
#else
	{ "SOCK_STREAM", 0, 1 },
#endif
};

int main(void)
{
	size_t i, j, n = sizeof v / sizeof v[0];

	pc_begin("XBD.sys_socket.h.004");
	pc_test();
	for (i = 0; i < n; i++) {
		pc_check(!v[i].missing, "the headers do not define %s", v[i].name);
		for (j = i + 1; j < n; j++)
			pc_check(v[i].missing || v[j].missing || v[i].value != v[j].value, "%s and %s are both %lld", v[i].name, v[j].name, v[i].value);
	}
	pc_pass();
}
