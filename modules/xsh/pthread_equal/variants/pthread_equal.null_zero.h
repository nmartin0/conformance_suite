/*
 * pthread_equal.null_zero.h - conforming variant (CI only): <pthread.h>
 * defines PTHREAD_NULL as a pthread_t whose bits are all zero.
 *
 * posixcheck: shim (a build shim: supplies declarations only; ci/mutate.sh
 * reports a failing run under it as SYSTEM, a verdict on the system).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * XBD <pthread.h> asks only for a constant expression that initializes
 * a pthread_t and equals no existing thread's ID. On the systems this
 * header is used with, a thread ID is never all zero bits (it is the
 * address of the thread's descriptor), so this is such a value; it lets
 * the test build where the C library does not yet define the name.
 */
#include <pthread.h>
#undef PTHREAD_NULL
#define PTHREAD_NULL ((pthread_t)0)
