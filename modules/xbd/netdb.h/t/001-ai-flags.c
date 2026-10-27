/*
 * The AI_ flags for ai_flags are bitwise-distinct.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.netdb.h.001
 */
#include <netdb.h>
#include "pc.h"


/* name, value, missing */
static const struct { const char *name; unsigned long long value; int missing; } f[] = {
#ifdef AI_PASSIVE
	{ "AI_PASSIVE", (unsigned long long)(AI_PASSIVE), 0 },
#else
	{ "AI_PASSIVE", 0, 1 },
#endif
#ifdef AI_CANONNAME
	{ "AI_CANONNAME", (unsigned long long)(AI_CANONNAME), 0 },
#else
	{ "AI_CANONNAME", 0, 1 },
#endif
#ifdef AI_NUMERICHOST
	{ "AI_NUMERICHOST", (unsigned long long)(AI_NUMERICHOST), 0 },
#else
	{ "AI_NUMERICHOST", 0, 1 },
#endif
#ifdef AI_NUMERICSERV
	{ "AI_NUMERICSERV", (unsigned long long)(AI_NUMERICSERV), 0 },
#else
	{ "AI_NUMERICSERV", 0, 1 },
#endif
#ifdef AI_V4MAPPED
	{ "AI_V4MAPPED", (unsigned long long)(AI_V4MAPPED), 0 },
#else
	{ "AI_V4MAPPED", 0, 1 },
#endif
#ifdef AI_ALL
	{ "AI_ALL", (unsigned long long)(AI_ALL), 0 },
#else
	{ "AI_ALL", 0, 1 },
#endif
#ifdef AI_ADDRCONFIG
	{ "AI_ADDRCONFIG", (unsigned long long)(AI_ADDRCONFIG), 0 },
#else
	{ "AI_ADDRCONFIG", 0, 1 },
#endif
};

int main(void)
{
	size_t i, j, n = sizeof f / sizeof f[0];

	pc_begin("XBD.netdb.h.001");
	pc_test();
	for (i = 0; i < n; i++) {
		pc_check(!f[i].missing, "<netdb.h> does not define %s", f[i].name);
		for (j = i + 1; j < n; j++)
			pc_check(f[i].missing || f[j].missing || (f[i].value & f[j].value) == 0, "%s (%#llx) and %s (%#llx) have bits in common",
			    f[i].name, f[i].value, f[j].name, f[j].value);
	}
	pc_pass();
}
