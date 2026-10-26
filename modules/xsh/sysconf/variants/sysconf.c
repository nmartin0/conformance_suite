/*
 * sysconf.c - conforming variants of sysconf(), for the variant checks
 * (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; see core/pcvariant.h. Each variant says why the
 * standard allows it.
 */
#define _GNU_SOURCE 1
#include <errno.h>
#include <limits.h>
#include <sys/resource.h>
#include <unistd.h>
#include "pcvariant.h"

/*
 * "no_limit": these maximums have no limit on this system: sysconf()
 * returns -1 and leaves errno alone. XSH sysconf() RETURN VALUE allows
 * exactly that for a variable <limits.h> describes as a maximum, and
 * <limits.h> omits a runtime invariant value that is indeterminate; a
 * name is used only where this system's <limits.h> does not define it, so
 * the compile-time and run-time values stay consistent.
 */
static int no_limit(int name)
{
	switch (name) {
#ifndef CHILD_MAX
	case _SC_CHILD_MAX:
#endif
#ifndef TIMER_MAX
	case _SC_TIMER_MAX:
#endif
#ifndef SIGQUEUE_MAX
	case _SC_SIGQUEUE_MAX:
#endif
#ifndef STREAM_MAX
	case _SC_STREAM_MAX:
#endif
#ifndef SEM_NSEMS_MAX
	case _SC_SEM_NSEMS_MAX:
#endif
#ifndef SYMLOOP_MAX
	case _SC_SYMLOOP_MAX:
#endif
#ifndef ATEXIT_MAX
	case _SC_ATEXIT_MAX:
#endif
#ifndef AIO_MAX
	case _SC_AIO_MAX:
#endif
#ifndef PTHREAD_THREADS_MAX
	case _SC_THREAD_THREADS_MAX:
#endif
		return 1;
	}
	return 0;
}

/*
 * "minimal": the limits that only describe utilities and other
 * interfaces' input sizes are exactly their minimum acceptable values (or
 * this system's <limits.h> value, where it defines a larger one). <limits.h>
 * lets an implementation choose any value not more restrictive than the
 * minimum, and sysconf() RETURN VALUE asks only that the value not be more
 * restrictive than the header's.
 */
#define AT_LEAST(h, min) ((h) > (min) ? (h) : (min))
#ifndef BC_BASE_MAX
#define BC_BASE_MAX 0	/* not in this <limits.h>: the minimum applies */
#endif
#ifndef BC_DIM_MAX
#define BC_DIM_MAX 0	/* not in this <limits.h>: the minimum applies */
#endif
#ifndef BC_SCALE_MAX
#define BC_SCALE_MAX 0	/* not in this <limits.h>: the minimum applies */
#endif
#ifndef BC_STRING_MAX
#define BC_STRING_MAX 0	/* not in this <limits.h>: the minimum applies */
#endif
#ifndef COLL_WEIGHTS_MAX
#define COLL_WEIGHTS_MAX 0	/* not in this <limits.h>: the minimum applies */
#endif
#ifndef EXPR_NEST_MAX
#define EXPR_NEST_MAX 0	/* not in this <limits.h>: the minimum applies */
#endif
#ifndef LINE_MAX
#define LINE_MAX 0	/* not in this <limits.h>: the minimum applies */
#endif
#ifndef RE_DUP_MAX
#define RE_DUP_MAX 0	/* not in this <limits.h>: the minimum applies */
#endif
#ifndef TZNAME_MAX
#define TZNAME_MAX 0	/* not in this <limits.h>: the minimum applies */
#endif

static int minimal_value(int name, long *out)
{
	switch (name) {
	case _SC_BC_BASE_MAX:
		*out = AT_LEAST(BC_BASE_MAX, 99);
		return 1;
	case _SC_BC_DIM_MAX:
		*out = AT_LEAST(BC_DIM_MAX, 2048);
		return 1;
	case _SC_BC_SCALE_MAX:
		*out = AT_LEAST(BC_SCALE_MAX, 99);
		return 1;
	case _SC_BC_STRING_MAX:
		*out = AT_LEAST(BC_STRING_MAX, 1000);
		return 1;
	case _SC_COLL_WEIGHTS_MAX:
		*out = AT_LEAST(COLL_WEIGHTS_MAX, 2);
		return 1;
	case _SC_EXPR_NEST_MAX:
		*out = AT_LEAST(EXPR_NEST_MAX, 32);
		return 1;
	case _SC_LINE_MAX:
		*out = AT_LEAST(LINE_MAX, 2048);
		return 1;
	case _SC_RE_DUP_MAX:
		*out = AT_LEAST(RE_DUP_MAX, 255);
		return 1;
	case _SC_TZNAME_MAX:
		*out = AT_LEAST(TZNAME_MAX, 6);
		return 1;
	}
	return 0;
}

/*
 * "errno_on_success": a successful call that returns a value other than
 * -1 leaves errno set to ENOENT. XSH 2.3 leaves errno unspecified after a
 * successful call unless the function's description says otherwise, and
 * sysconf() promises an unchanged errno only for -1 ("no limit").
 *
 * "open_max_hard": sysconf(_SC_OPEN_MAX) reports the RLIMIT_NOFILE hard
 * limit (or -1 when it is infinite). The value must only exceed every
 * descriptor the system may assign (<limits.h> {OPEN_MAX}); the soft limit
 * never exceeds the hard one, and the RETURN VALUE lets the value change
 * after setrlimit() or stay as it is.
 */
long sysconf(int name)
{
	const char *v = variant_of("sysconf.");
	struct rlimit rl;
	long r;
	int e = errno;
	REAL(sysconf);

	if (IS("no_limit") && no_limit(name))
		return -1;
	if (IS("minimal") && minimal_value(name, &r))
		return r;
	if (IS("open_max_hard") && name == _SC_OPEN_MAX) {
		if (getrlimit(RLIMIT_NOFILE, &rl) != 0)
			return real_sysconf(name);
		if (rl.rlim_max == RLIM_INFINITY || rl.rlim_max > LONG_MAX) {
			errno = e;
			return -1;
		}
		return (long)rl.rlim_max;
	}
	r = real_sysconf(name);
	if (IS("errno_on_success") && r != -1)
		errno = ENOENT;
	return r;
}
