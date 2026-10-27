/*
 * The SOCK_ creation flags are bitwise distinct from each other and from
 *   the socket type constants.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sys_socket.h.006
 */
#include <sys/socket.h>
#include "pc.h"


/* name, value, missing */
static const struct { const char *name; unsigned long long value; int missing; } flag[] = {
#ifdef SOCK_NONBLOCK
	{ "SOCK_NONBLOCK", (unsigned long long)(SOCK_NONBLOCK), 0 },
#else
	{ "SOCK_NONBLOCK", 0, 1 },
#endif
#ifdef SOCK_CLOEXEC
	{ "SOCK_CLOEXEC", (unsigned long long)(SOCK_CLOEXEC), 0 },
#else
	{ "SOCK_CLOEXEC", 0, 1 },
#endif
#ifdef SOCK_CLOFORK
	{ "SOCK_CLOFORK", (unsigned long long)(SOCK_CLOFORK), 0 },
#else
	{ "SOCK_CLOFORK", 0, 1 },
#endif
};
static const struct { const char *name; unsigned long long value; } type[] = {
	{ "SOCK_DGRAM", SOCK_DGRAM },
	{ "SOCK_SEQPACKET", SOCK_SEQPACKET },
	{ "SOCK_STREAM", SOCK_STREAM },
#ifdef SOCK_RAW
	{ "SOCK_RAW", SOCK_RAW },
#endif
};

int main(void)
{
	size_t i, j, nf = sizeof flag / sizeof flag[0], nt = sizeof type / sizeof type[0];

	pc_begin("XBD.sys_socket.h.006");
	pc_test();
	for (i = 0; i < nf; i++) {
		pc_check(!flag[i].missing, "<sys/socket.h> does not define %s", flag[i].name);
		for (j = i + 1; j < nf; j++)
			pc_check(flag[j].missing || (flag[i].value & flag[j].value) == 0, "%s (%#llx) and %s (%#llx) have bits in common",
			    flag[i].name, flag[i].value, flag[j].name, flag[j].value);
		for (j = 0; j < nt; j++)
			pc_check((flag[i].value & type[j].value) == 0, "%s (%#llx) and %s (%#llx) have bits in common",
			    flag[i].name, flag[i].value, type[j].name, type[j].value);
	}
	pc_pass();
}
