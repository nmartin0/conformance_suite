/*
 * sigaddset.c - conforming variants for the signal-set functions, for the
 * variant checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; see core/pcvariant.h. Each variant says why the
 * standard allows it.
 */
#define _GNU_SOURCE 1
#include <unistd.h>
#include "pcvariant.h"

/*
 * "rt_minimum": {RTSIG_MAX} is 8, its least allowed value (XBD
 * <limits.h>), to agree with the realtime range of exactly eight signals
 * that sigaddset.rt_minimum.h defines.
 */
long sysconf(int name)
{
	const char *v = variant_of("sigaddset.");
	REAL(sysconf);

	if (IS("rt_minimum") && name == _SC_RTSIG_MAX)
		return 8;
	return real_sysconf(name);
}
