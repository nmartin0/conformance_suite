/*
 * A sporadic server option goes with the matching scheduling option.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.2-conformance.007
 */
#include <unistd.h>
#include "pc.h"

#if defined(_POSIX_SPORADIC_SERVER) && _POSIX_SPORADIC_SERVER == 202405L
#define SS 1
#else
#define SS 0
#endif
#if defined(_POSIX_PRIORITY_SCHEDULING) && _POSIX_PRIORITY_SCHEDULING == 202405L
#define PS 1
#else
#define PS 0
#endif
#if defined(_POSIX_THREAD_SPORADIC_SERVER) && \
    _POSIX_THREAD_SPORADIC_SERVER == 202405L
#define TSP 1
#else
#define TSP 0
#endif
#if defined(_POSIX_THREAD_PRIORITY_SCHEDULING) && \
    _POSIX_THREAD_PRIORITY_SCHEDULING == 202405L
#define TPS 1
#else
#define TPS 0
#endif

int main(void)
{
	pc_begin("XBD.2-conformance.007");
	if (!SS && !TSP)
		pc_notinuse("neither sporadic server option is defined as "
		    "202405L");
	pc_test();
	if (SS)
		pc_check(PS, "_POSIX_SPORADIC_SERVER is 202405L but "
		    "_POSIX_PRIORITY_SCHEDULING is not");
	if (TSP)
		pc_check(TPS, "_POSIX_THREAD_SPORADIC_SERVER is 202405L but "
		    "_POSIX_THREAD_PRIORITY_SCHEDULING is not");
	pc_pass();
}
