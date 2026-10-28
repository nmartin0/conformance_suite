/*
 * An XSI system always supports the file synchronization, thread stack
 * address, thread stack size and thread process-shared options.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.2-conformance.004
 */
#include <unistd.h>
#include "pc.h"

#if defined(_POSIX_FSYNC) && _POSIX_FSYNC > 0
#define FSYNC_OK 1
#else
#define FSYNC_OK 0
#endif
#if defined(_POSIX_THREAD_ATTR_STACKADDR) && _POSIX_THREAD_ATTR_STACKADDR > 0
#define STACKADDR_OK 1
#else
#define STACKADDR_OK 0
#endif
#if defined(_POSIX_THREAD_ATTR_STACKSIZE) && _POSIX_THREAD_ATTR_STACKSIZE > 0
#define STACKSIZE_OK 1
#else
#define STACKSIZE_OK 0
#endif
#if defined(_POSIX_THREAD_PROCESS_SHARED) && _POSIX_THREAD_PROCESS_SHARED > 0
#define PSHARED_OK 1
#else
#define PSHARED_OK 0
#endif

int main(void)
{
	pc_begin("XBD.2-conformance.004");
	pc_test();
	pc_check(FSYNC_OK, "_POSIX_FSYNC is not defined above zero");
	pc_check(STACKADDR_OK,
	    "_POSIX_THREAD_ATTR_STACKADDR is not defined above zero");
	pc_check(STACKSIZE_OK,
	    "_POSIX_THREAD_ATTR_STACKSIZE is not defined above zero");
	pc_check(PSHARED_OK,
	    "_POSIX_THREAD_PROCESS_SHARED is not defined above zero");
	pc_pass();
}
