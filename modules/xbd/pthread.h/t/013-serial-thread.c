/*
 * PTHREAD_BARRIER_SERIAL_THREAD is nonzero and differs from EINVAL, the
 * error the page names for pthread_barrier_wait().
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.pthread.h.013
 */
#include "pc.h"
#include <errno.h>
#include <pthread.h>

int main(void)
{
	pc_begin("XBD.pthread.h.013");
#ifndef PTHREAD_BARRIER_SERIAL_THREAD
	pc_unresolved("PTHREAD_BARRIER_SERIAL_THREAD is missing (the header check reports it)");
#else
	pc_test();
	pc_check((PTHREAD_BARRIER_SERIAL_THREAD) != 0, "PTHREAD_BARRIER_SERIAL_THREAD is zero, the value the other threads get");
	pc_check((PTHREAD_BARRIER_SERIAL_THREAD) != EINVAL, "PTHREAD_BARRIER_SERIAL_THREAD equals EINVAL");
	pc_pass();
#endif
}
