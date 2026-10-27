/*
 * Always-supported option symbols.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.unistd.h.007
 */
#include <unistd.h>
#include "pc.h"
#ifndef _POSIX_SUBPROFILE
#if !defined(_POSIX_ASYNCHRONOUS_IO) || _POSIX_ASYNCHRONOUS_IO != 202405L
#error "_POSIX_ASYNCHRONOUS_IO is not defined as 202405L"
#endif
#if !defined(_POSIX_BARRIERS) || _POSIX_BARRIERS != 202405L
#error "_POSIX_BARRIERS is not defined as 202405L"
#endif
#if !defined(_POSIX_CLOCK_SELECTION) || _POSIX_CLOCK_SELECTION != 202405L
#error "_POSIX_CLOCK_SELECTION is not defined as 202405L"
#endif
#if !defined(_POSIX_MAPPED_FILES) || _POSIX_MAPPED_FILES != 202405L
#error "_POSIX_MAPPED_FILES is not defined as 202405L"
#endif
#if !defined(_POSIX_MEMORY_PROTECTION) || _POSIX_MEMORY_PROTECTION != 202405L
#error "_POSIX_MEMORY_PROTECTION is not defined as 202405L"
#endif
#if !defined(_POSIX_MONOTONIC_CLOCK) || _POSIX_MONOTONIC_CLOCK != 202405L
#error "_POSIX_MONOTONIC_CLOCK is not defined as 202405L"
#endif
#if !defined(_POSIX_READER_WRITER_LOCKS) || _POSIX_READER_WRITER_LOCKS != 202405L
#error "_POSIX_READER_WRITER_LOCKS is not defined as 202405L"
#endif
#if !defined(_POSIX_REALTIME_SIGNALS) || _POSIX_REALTIME_SIGNALS != 202405L
#error "_POSIX_REALTIME_SIGNALS is not defined as 202405L"
#endif
#if !defined(_POSIX_SEMAPHORES) || _POSIX_SEMAPHORES != 202405L
#error "_POSIX_SEMAPHORES is not defined as 202405L"
#endif
#if !defined(_POSIX_SPIN_LOCKS) || _POSIX_SPIN_LOCKS != 202405L
#error "_POSIX_SPIN_LOCKS is not defined as 202405L"
#endif
#if !defined(_POSIX_THREAD_SAFE_FUNCTIONS) || _POSIX_THREAD_SAFE_FUNCTIONS != 202405L
#error "_POSIX_THREAD_SAFE_FUNCTIONS is not defined as 202405L"
#endif
#if !defined(_POSIX_THREADS) || _POSIX_THREADS != 202405L
#error "_POSIX_THREADS is not defined as 202405L"
#endif
#if !defined(_POSIX_TIMEOUTS) || _POSIX_TIMEOUTS != 202405L
#error "_POSIX_TIMEOUTS is not defined as 202405L"
#endif
#if !defined(_POSIX_TIMERS) || _POSIX_TIMERS != 202405L
#error "_POSIX_TIMERS is not defined as 202405L"
#endif
#if !defined(_POSIX2_C_BIND) || _POSIX2_C_BIND != 202405L
#error "_POSIX2_C_BIND is not defined as 202405L"
#endif
#if !defined(_POSIX_JOB_CONTROL) || _POSIX_JOB_CONTROL <= 0
#error "_POSIX_JOB_CONTROL is not defined with a value greater than zero"
#endif
#if !defined(_POSIX_REGEXP) || _POSIX_REGEXP <= 0
#error "_POSIX_REGEXP is not defined with a value greater than zero"
#endif
#if !defined(_POSIX_SAVED_IDS) || _POSIX_SAVED_IDS <= 0
#error "_POSIX_SAVED_IDS is not defined with a value greater than zero"
#endif
#if !defined(_POSIX_SHELL) || _POSIX_SHELL <= 0
#error "_POSIX_SHELL is not defined with a value greater than zero"
#endif
#else
#if defined(_POSIX_ASYNCHRONOUS_IO) && _POSIX_ASYNCHRONOUS_IO < -1
#error "_POSIX_ASYNCHRONOUS_IO is below -1"
#endif
#if defined(_POSIX_BARRIERS) && _POSIX_BARRIERS < -1
#error "_POSIX_BARRIERS is below -1"
#endif
#if defined(_POSIX_CLOCK_SELECTION) && _POSIX_CLOCK_SELECTION < -1
#error "_POSIX_CLOCK_SELECTION is below -1"
#endif
#if defined(_POSIX_MAPPED_FILES) && _POSIX_MAPPED_FILES < -1
#error "_POSIX_MAPPED_FILES is below -1"
#endif
#if defined(_POSIX_MEMORY_PROTECTION) && _POSIX_MEMORY_PROTECTION < -1
#error "_POSIX_MEMORY_PROTECTION is below -1"
#endif
#if defined(_POSIX_MONOTONIC_CLOCK) && _POSIX_MONOTONIC_CLOCK < -1
#error "_POSIX_MONOTONIC_CLOCK is below -1"
#endif
#if defined(_POSIX_READER_WRITER_LOCKS) && _POSIX_READER_WRITER_LOCKS < -1
#error "_POSIX_READER_WRITER_LOCKS is below -1"
#endif
#if defined(_POSIX_REALTIME_SIGNALS) && _POSIX_REALTIME_SIGNALS < -1
#error "_POSIX_REALTIME_SIGNALS is below -1"
#endif
#if defined(_POSIX_SEMAPHORES) && _POSIX_SEMAPHORES < -1
#error "_POSIX_SEMAPHORES is below -1"
#endif
#if defined(_POSIX_SPIN_LOCKS) && _POSIX_SPIN_LOCKS < -1
#error "_POSIX_SPIN_LOCKS is below -1"
#endif
#if defined(_POSIX_THREAD_SAFE_FUNCTIONS) && _POSIX_THREAD_SAFE_FUNCTIONS < -1
#error "_POSIX_THREAD_SAFE_FUNCTIONS is below -1"
#endif
#if defined(_POSIX_THREADS) && _POSIX_THREADS < -1
#error "_POSIX_THREADS is below -1"
#endif
#if defined(_POSIX_TIMEOUTS) && _POSIX_TIMEOUTS < -1
#error "_POSIX_TIMEOUTS is below -1"
#endif
#if defined(_POSIX_TIMERS) && _POSIX_TIMERS < -1
#error "_POSIX_TIMERS is below -1"
#endif
#if defined(_POSIX2_C_BIND) && _POSIX2_C_BIND < -1
#error "_POSIX2_C_BIND is below -1"
#endif
#if defined(_POSIX_JOB_CONTROL) && _POSIX_JOB_CONTROL < -1
#error "_POSIX_JOB_CONTROL is below -1"
#endif
#if defined(_POSIX_REGEXP) && _POSIX_REGEXP < -1
#error "_POSIX_REGEXP is below -1"
#endif
#if defined(_POSIX_SAVED_IDS) && _POSIX_SAVED_IDS < -1
#error "_POSIX_SAVED_IDS is below -1"
#endif
#if defined(_POSIX_SHELL) && _POSIX_SHELL < -1
#error "_POSIX_SHELL is below -1"
#endif
#endif

int main(void)
{
	pc_begin("XBD.unistd.h.007");
	pc_test();
	pc_pass();
}
