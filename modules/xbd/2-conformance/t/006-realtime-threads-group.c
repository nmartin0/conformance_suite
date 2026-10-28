/*
 * The Realtime Threads Option Group, when claimed, defines its options as
 * 202405L.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.2-conformance.006
 */
#include <unistd.h>
#include "pc.h"

#if defined(_XOPEN_REALTIME_THREADS) && _XOPEN_REALTIME_THREADS != -1
#define CLAIMED 1
#else
#define CLAIMED 0
#endif

#if defined(_POSIX_THREAD_PRIO_INHERIT) && \
    _POSIX_THREAD_PRIO_INHERIT == 202405L
#define PRIO_INHERIT_OK 1
#else
#define PRIO_INHERIT_OK 0
#endif
#if defined(_POSIX_THREAD_PRIO_PROTECT) && \
    _POSIX_THREAD_PRIO_PROTECT == 202405L
#define PRIO_PROTECT_OK 1
#else
#define PRIO_PROTECT_OK 0
#endif
#if defined(_POSIX_THREAD_PRIORITY_SCHEDULING) && \
    _POSIX_THREAD_PRIORITY_SCHEDULING == 202405L
#define PRIORITY_SCHEDULING_OK 1
#else
#define PRIORITY_SCHEDULING_OK 0
#endif
#if defined(_POSIX_THREAD_ROBUST_PRIO_INHERIT) && \
    _POSIX_THREAD_ROBUST_PRIO_INHERIT == 202405L
#define ROBUST_INHERIT_OK 1
#else
#define ROBUST_INHERIT_OK 0
#endif
#if defined(_POSIX_THREAD_ROBUST_PRIO_PROTECT) && \
    _POSIX_THREAD_ROBUST_PRIO_PROTECT == 202405L
#define ROBUST_PROTECT_OK 1
#else
#define ROBUST_PROTECT_OK 0
#endif

int main(void)
{
	pc_begin("XBD.2-conformance.006");
	if (!CLAIMED)
		pc_notinuse("_XOPEN_REALTIME_THREADS is not defined or is -1");
	pc_test();
	pc_check(PRIO_INHERIT_OK, "_POSIX_THREAD_PRIO_INHERIT is not 202405L");
	pc_check(PRIO_PROTECT_OK, "_POSIX_THREAD_PRIO_PROTECT is not 202405L");
	pc_check(PRIORITY_SCHEDULING_OK,
	    "_POSIX_THREAD_PRIORITY_SCHEDULING is not 202405L");
	pc_check(ROBUST_INHERIT_OK,
	    "_POSIX_THREAD_ROBUST_PRIO_INHERIT is not 202405L");
	pc_check(ROBUST_PROTECT_OK,
	    "_POSIX_THREAD_ROBUST_PRIO_PROTECT is not 202405L");
	pc_pass();
}
