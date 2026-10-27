/*
 * The NI_ flags for getnameinfo() are bitwise-distinct.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.netdb.h.002
 */
#include <netdb.h>
#include "pc.h"


/* name, value, missing */
static const struct { const char *name; unsigned long long value; int missing; } f[] = {
#ifdef NI_NOFQDN
	{ "NI_NOFQDN", (unsigned long long)(NI_NOFQDN), 0 },
#else
	{ "NI_NOFQDN", 0, 1 },
#endif
#ifdef NI_NUMERICHOST
	{ "NI_NUMERICHOST", (unsigned long long)(NI_NUMERICHOST), 0 },
#else
	{ "NI_NUMERICHOST", 0, 1 },
#endif
#ifdef NI_NAMEREQD
	{ "NI_NAMEREQD", (unsigned long long)(NI_NAMEREQD), 0 },
#else
	{ "NI_NAMEREQD", 0, 1 },
#endif
#ifdef NI_NUMERICSERV
	{ "NI_NUMERICSERV", (unsigned long long)(NI_NUMERICSERV), 0 },
#else
	{ "NI_NUMERICSERV", 0, 1 },
#endif
#ifdef NI_NUMERICSCOPE
	{ "NI_NUMERICSCOPE", (unsigned long long)(NI_NUMERICSCOPE), 0 },
#else
	{ "NI_NUMERICSCOPE", 0, 1 },
#endif
#ifdef NI_DGRAM
	{ "NI_DGRAM", (unsigned long long)(NI_DGRAM), 0 },
#else
	{ "NI_DGRAM", 0, 1 },
#endif
};

int main(void)
{
	size_t i, j, n = sizeof f / sizeof f[0];

	pc_begin("XBD.netdb.h.002");
	pc_test();
	for (i = 0; i < n; i++) {
		pc_check(!f[i].missing, "<netdb.h> does not define %s", f[i].name);
		for (j = i + 1; j < n; j++)
			pc_check(f[i].missing || f[j].missing || (f[i].value & f[j].value) == 0, "%s (%#llx) and %s (%#llx) have bits in common",
			    f[i].name, f[i].value, f[j].name, f[j].value);
	}
	pc_pass();
}
