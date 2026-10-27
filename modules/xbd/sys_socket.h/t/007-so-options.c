/*
 * The SO_ socket options have distinct values.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sys_socket.h.007
 */
#include <sys/socket.h>
#include "pc.h"


/* name, value, missing */
static const struct { const char *name; long long value; int missing; } v[] = {
#ifdef SO_ACCEPTCONN
	{ "SO_ACCEPTCONN", (long long)(SO_ACCEPTCONN), 0 },
#else
	{ "SO_ACCEPTCONN", 0, 1 },
#endif
#ifdef SO_BROADCAST
	{ "SO_BROADCAST", (long long)(SO_BROADCAST), 0 },
#else
	{ "SO_BROADCAST", 0, 1 },
#endif
#ifdef SO_DEBUG
	{ "SO_DEBUG", (long long)(SO_DEBUG), 0 },
#else
	{ "SO_DEBUG", 0, 1 },
#endif
#ifdef SO_DOMAIN
	{ "SO_DOMAIN", (long long)(SO_DOMAIN), 0 },
#else
	{ "SO_DOMAIN", 0, 1 },
#endif
#ifdef SO_DONTROUTE
	{ "SO_DONTROUTE", (long long)(SO_DONTROUTE), 0 },
#else
	{ "SO_DONTROUTE", 0, 1 },
#endif
#ifdef SO_ERROR
	{ "SO_ERROR", (long long)(SO_ERROR), 0 },
#else
	{ "SO_ERROR", 0, 1 },
#endif
#ifdef SO_KEEPALIVE
	{ "SO_KEEPALIVE", (long long)(SO_KEEPALIVE), 0 },
#else
	{ "SO_KEEPALIVE", 0, 1 },
#endif
#ifdef SO_LINGER
	{ "SO_LINGER", (long long)(SO_LINGER), 0 },
#else
	{ "SO_LINGER", 0, 1 },
#endif
#ifdef SO_OOBINLINE
	{ "SO_OOBINLINE", (long long)(SO_OOBINLINE), 0 },
#else
	{ "SO_OOBINLINE", 0, 1 },
#endif
#ifdef SO_PROTOCOL
	{ "SO_PROTOCOL", (long long)(SO_PROTOCOL), 0 },
#else
	{ "SO_PROTOCOL", 0, 1 },
#endif
#ifdef SO_RCVBUF
	{ "SO_RCVBUF", (long long)(SO_RCVBUF), 0 },
#else
	{ "SO_RCVBUF", 0, 1 },
#endif
#ifdef SO_RCVLOWAT
	{ "SO_RCVLOWAT", (long long)(SO_RCVLOWAT), 0 },
#else
	{ "SO_RCVLOWAT", 0, 1 },
#endif
#ifdef SO_RCVTIMEO
	{ "SO_RCVTIMEO", (long long)(SO_RCVTIMEO), 0 },
#else
	{ "SO_RCVTIMEO", 0, 1 },
#endif
#ifdef SO_REUSEADDR
	{ "SO_REUSEADDR", (long long)(SO_REUSEADDR), 0 },
#else
	{ "SO_REUSEADDR", 0, 1 },
#endif
#ifdef SO_SNDBUF
	{ "SO_SNDBUF", (long long)(SO_SNDBUF), 0 },
#else
	{ "SO_SNDBUF", 0, 1 },
#endif
#ifdef SO_SNDLOWAT
	{ "SO_SNDLOWAT", (long long)(SO_SNDLOWAT), 0 },
#else
	{ "SO_SNDLOWAT", 0, 1 },
#endif
#ifdef SO_SNDTIMEO
	{ "SO_SNDTIMEO", (long long)(SO_SNDTIMEO), 0 },
#else
	{ "SO_SNDTIMEO", 0, 1 },
#endif
#ifdef SO_TYPE
	{ "SO_TYPE", (long long)(SO_TYPE), 0 },
#else
	{ "SO_TYPE", 0, 1 },
#endif
};

int main(void)
{
	size_t i, j, n = sizeof v / sizeof v[0];

	pc_begin("XBD.sys_socket.h.007");
	pc_test();
	for (i = 0; i < n; i++) {
		pc_check(!v[i].missing, "the headers do not define %s", v[i].name);
		for (j = i + 1; j < n; j++)
			pc_check(v[i].missing || v[j].missing || v[i].value != v[j].value, "%s and %s are both %lld", v[i].name, v[j].name, v[i].value);
	}
	pc_pass();
}
