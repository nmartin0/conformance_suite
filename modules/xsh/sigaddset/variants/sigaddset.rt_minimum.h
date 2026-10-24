/*
 * sigaddset.rt_minimum.h - conforming variant: the realtime signal range
 * is as small as allowed and starts two signals higher than the C
 * library's (CI only; with variants/sigaddset.c).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * XBD <signal.h> asks only that SIGRTMIN through SIGRTMAX hold at least
 * {RTSIG_MAX} signal numbers, overlap none of the named signals, and that
 * SIGRTMAX be below sysconf(_SC_NSIG); XBD <limits.h> lets {RTSIG_MAX} be
 * as low as {_POSIX_RTSIG_MAX}, 8. Here SIGRTMIN is the library's plus 2
 * and SIGRTMAX is SIGRTMIN + 7; the library's other realtime numbers are
 * then implementation-defined signals of this system, for which realtime
 * behaviour may or may not be supported (XBD <signal.h>). RTSIG_MAX and
 * sysconf(_SC_RTSIG_MAX) (in the preload library) say 8.
 */
#include <limits.h>
#include <signal.h>

static inline int pcv_library_rtmin(void)
{
	return SIGRTMIN;
}

#undef SIGRTMIN
#undef SIGRTMAX
#define SIGRTMIN (pcv_library_rtmin() + 2)
#define SIGRTMAX (SIGRTMIN + 7)
#undef RTSIG_MAX
#define RTSIG_MAX 8
