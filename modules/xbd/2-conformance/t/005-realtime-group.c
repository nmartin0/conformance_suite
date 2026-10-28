/*
 * The Realtime Option Group, when claimed, defines its options as 202405L.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.2-conformance.005
 */
#include <unistd.h>
#include "pc.h"

#if defined(_XOPEN_REALTIME) && _XOPEN_REALTIME != -1
#define CLAIMED 1
#else
#define CLAIMED 0
#endif

#if defined(_POSIX_MEMLOCK) && _POSIX_MEMLOCK == 202405L
#define MEMLOCK_OK 1
#else
#define MEMLOCK_OK 0
#endif
#if defined(_POSIX_MEMLOCK_RANGE) && _POSIX_MEMLOCK_RANGE == 202405L
#define MEMLOCK_RANGE_OK 1
#else
#define MEMLOCK_RANGE_OK 0
#endif
#if defined(_POSIX_MESSAGE_PASSING) && _POSIX_MESSAGE_PASSING == 202405L
#define MESSAGE_PASSING_OK 1
#else
#define MESSAGE_PASSING_OK 0
#endif
#if defined(_POSIX_PRIORITY_SCHEDULING) && _POSIX_PRIORITY_SCHEDULING == 202405L
#define PRIORITY_SCHEDULING_OK 1
#else
#define PRIORITY_SCHEDULING_OK 0
#endif
#if defined(_POSIX_SHARED_MEMORY_OBJECTS) && \
    _POSIX_SHARED_MEMORY_OBJECTS == 202405L
#define SHARED_MEMORY_OK 1
#else
#define SHARED_MEMORY_OK 0
#endif
#if defined(_POSIX_SYNCHRONIZED_IO) && _POSIX_SYNCHRONIZED_IO == 202405L
#define SYNCHRONIZED_IO_OK 1
#else
#define SYNCHRONIZED_IO_OK 0
#endif

int main(void)
{
	pc_begin("XBD.2-conformance.005");
	if (!CLAIMED)
		pc_notinuse("_XOPEN_REALTIME is not defined or is -1");
	pc_test();
	pc_check(MEMLOCK_OK, "_POSIX_MEMLOCK is not 202405L");
	pc_check(MEMLOCK_RANGE_OK, "_POSIX_MEMLOCK_RANGE is not 202405L");
	pc_check(MESSAGE_PASSING_OK, "_POSIX_MESSAGE_PASSING is not 202405L");
	pc_check(PRIORITY_SCHEDULING_OK,
	    "_POSIX_PRIORITY_SCHEDULING is not 202405L");
	pc_check(SHARED_MEMORY_OK,
	    "_POSIX_SHARED_MEMORY_OBJECTS is not 202405L");
	pc_check(SYNCHRONIZED_IO_OK, "_POSIX_SYNCHRONIZED_IO is not 202405L");
	pc_pass();
}
