/*
 * A positive option symbol is not contradicted by sysconf().
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.unistd.h.011
 */
#include <unistd.h>
#include "pc.h"

int main(void)
{
	long r;

	pc_begin("XBD.unistd.h.011");
	pc_test();
#if defined(_POSIX2_CHAR_TERM) && _POSIX2_CHAR_TERM > 0
	r = sysconf(_SC_2_CHAR_TERM);
	pc_check(r != -1, "_POSIX2_CHAR_TERM is greater than zero but sysconf(_SC_2_CHAR_TERM) is -1");
#endif
#if defined(_POSIX2_C_BIND) && _POSIX2_C_BIND > 0
	r = sysconf(_SC_2_C_BIND);
	pc_check(r != -1, "_POSIX2_C_BIND is greater than zero but sysconf(_SC_2_C_BIND) is -1");
#endif
#if defined(_POSIX2_C_DEV) && _POSIX2_C_DEV > 0
	r = sysconf(_SC_2_C_DEV);
	pc_check(r != -1, "_POSIX2_C_DEV is greater than zero but sysconf(_SC_2_C_DEV) is -1");
#endif
#if defined(_POSIX2_FORT_RUN) && _POSIX2_FORT_RUN > 0
	r = sysconf(_SC_2_FORT_RUN);
	pc_check(r != -1, "_POSIX2_FORT_RUN is greater than zero but sysconf(_SC_2_FORT_RUN) is -1");
#endif
#if defined(_POSIX2_LOCALEDEF) && _POSIX2_LOCALEDEF > 0
	r = sysconf(_SC_2_LOCALEDEF);
	pc_check(r != -1, "_POSIX2_LOCALEDEF is greater than zero but sysconf(_SC_2_LOCALEDEF) is -1");
#endif
#if defined(_POSIX2_SW_DEV) && _POSIX2_SW_DEV > 0
	r = sysconf(_SC_2_SW_DEV);
	pc_check(r != -1, "_POSIX2_SW_DEV is greater than zero but sysconf(_SC_2_SW_DEV) is -1");
#endif
#if defined(_POSIX2_UPE) && _POSIX2_UPE > 0
	r = sysconf(_SC_2_UPE);
	pc_check(r != -1, "_POSIX2_UPE is greater than zero but sysconf(_SC_2_UPE) is -1");
#endif
#if defined(_POSIX_ADVISORY_INFO) && _POSIX_ADVISORY_INFO > 0
	r = sysconf(_SC_ADVISORY_INFO);
	pc_check(r != -1, "_POSIX_ADVISORY_INFO is greater than zero but sysconf(_SC_ADVISORY_INFO) is -1");
#endif
#if defined(_POSIX_ASYNCHRONOUS_IO) && _POSIX_ASYNCHRONOUS_IO > 0
	r = sysconf(_SC_ASYNCHRONOUS_IO);
	pc_check(r != -1, "_POSIX_ASYNCHRONOUS_IO is greater than zero but sysconf(_SC_ASYNCHRONOUS_IO) is -1");
#endif
#if defined(_POSIX_BARRIERS) && _POSIX_BARRIERS > 0
	r = sysconf(_SC_BARRIERS);
	pc_check(r != -1, "_POSIX_BARRIERS is greater than zero but sysconf(_SC_BARRIERS) is -1");
#endif
#if defined(_POSIX_CLOCK_SELECTION) && _POSIX_CLOCK_SELECTION > 0
	r = sysconf(_SC_CLOCK_SELECTION);
	pc_check(r != -1, "_POSIX_CLOCK_SELECTION is greater than zero but sysconf(_SC_CLOCK_SELECTION) is -1");
#endif
#if defined(_POSIX_CPUTIME) && _POSIX_CPUTIME > 0
	r = sysconf(_SC_CPUTIME);
	pc_check(r != -1, "_POSIX_CPUTIME is greater than zero but sysconf(_SC_CPUTIME) is -1");
#endif
#if defined(_POSIX_DEVICE_CONTROL) && _POSIX_DEVICE_CONTROL > 0
	r = sysconf(_SC_DEVICE_CONTROL);
	pc_check(r != -1, "_POSIX_DEVICE_CONTROL is greater than zero but sysconf(_SC_DEVICE_CONTROL) is -1");
#endif
#if defined(_POSIX_FSYNC) && _POSIX_FSYNC > 0
	r = sysconf(_SC_FSYNC);
	pc_check(r != -1, "_POSIX_FSYNC is greater than zero but sysconf(_SC_FSYNC) is -1");
#endif
#if defined(_POSIX_IPV6) && _POSIX_IPV6 > 0
	r = sysconf(_SC_IPV6);
	pc_check(r != -1, "_POSIX_IPV6 is greater than zero but sysconf(_SC_IPV6) is -1");
#endif
#if defined(_POSIX_JOB_CONTROL) && _POSIX_JOB_CONTROL > 0
	r = sysconf(_SC_JOB_CONTROL);
	pc_check(r != -1, "_POSIX_JOB_CONTROL is greater than zero but sysconf(_SC_JOB_CONTROL) is -1");
#endif
#if defined(_POSIX_MAPPED_FILES) && _POSIX_MAPPED_FILES > 0
	r = sysconf(_SC_MAPPED_FILES);
	pc_check(r != -1, "_POSIX_MAPPED_FILES is greater than zero but sysconf(_SC_MAPPED_FILES) is -1");
#endif
#if defined(_POSIX_MEMLOCK) && _POSIX_MEMLOCK > 0
	r = sysconf(_SC_MEMLOCK);
	pc_check(r != -1, "_POSIX_MEMLOCK is greater than zero but sysconf(_SC_MEMLOCK) is -1");
#endif
#if defined(_POSIX_MEMLOCK_RANGE) && _POSIX_MEMLOCK_RANGE > 0
	r = sysconf(_SC_MEMLOCK_RANGE);
	pc_check(r != -1, "_POSIX_MEMLOCK_RANGE is greater than zero but sysconf(_SC_MEMLOCK_RANGE) is -1");
#endif
#if defined(_POSIX_MEMORY_PROTECTION) && _POSIX_MEMORY_PROTECTION > 0
	r = sysconf(_SC_MEMORY_PROTECTION);
	pc_check(r != -1, "_POSIX_MEMORY_PROTECTION is greater than zero but sysconf(_SC_MEMORY_PROTECTION) is -1");
#endif
#if defined(_POSIX_MESSAGE_PASSING) && _POSIX_MESSAGE_PASSING > 0
	r = sysconf(_SC_MESSAGE_PASSING);
	pc_check(r != -1, "_POSIX_MESSAGE_PASSING is greater than zero but sysconf(_SC_MESSAGE_PASSING) is -1");
#endif
#if defined(_POSIX_MONOTONIC_CLOCK) && _POSIX_MONOTONIC_CLOCK > 0
	r = sysconf(_SC_MONOTONIC_CLOCK);
	pc_check(r != -1, "_POSIX_MONOTONIC_CLOCK is greater than zero but sysconf(_SC_MONOTONIC_CLOCK) is -1");
#endif
#if defined(_POSIX_PRIORITIZED_IO) && _POSIX_PRIORITIZED_IO > 0
	r = sysconf(_SC_PRIORITIZED_IO);
	pc_check(r != -1, "_POSIX_PRIORITIZED_IO is greater than zero but sysconf(_SC_PRIORITIZED_IO) is -1");
#endif
#if defined(_POSIX_PRIORITY_SCHEDULING) && _POSIX_PRIORITY_SCHEDULING > 0
	r = sysconf(_SC_PRIORITY_SCHEDULING);
	pc_check(r != -1, "_POSIX_PRIORITY_SCHEDULING is greater than zero but sysconf(_SC_PRIORITY_SCHEDULING) is -1");
#endif
#if defined(_POSIX_RAW_SOCKETS) && _POSIX_RAW_SOCKETS > 0
	r = sysconf(_SC_RAW_SOCKETS);
	pc_check(r != -1, "_POSIX_RAW_SOCKETS is greater than zero but sysconf(_SC_RAW_SOCKETS) is -1");
#endif
#if defined(_POSIX_READER_WRITER_LOCKS) && _POSIX_READER_WRITER_LOCKS > 0
	r = sysconf(_SC_READER_WRITER_LOCKS);
	pc_check(r != -1, "_POSIX_READER_WRITER_LOCKS is greater than zero but sysconf(_SC_READER_WRITER_LOCKS) is -1");
#endif
#if defined(_POSIX_REALTIME_SIGNALS) && _POSIX_REALTIME_SIGNALS > 0
	r = sysconf(_SC_REALTIME_SIGNALS);
	pc_check(r != -1, "_POSIX_REALTIME_SIGNALS is greater than zero but sysconf(_SC_REALTIME_SIGNALS) is -1");
#endif
#if defined(_POSIX_REGEXP) && _POSIX_REGEXP > 0
	r = sysconf(_SC_REGEXP);
	pc_check(r != -1, "_POSIX_REGEXP is greater than zero but sysconf(_SC_REGEXP) is -1");
#endif
#if defined(_POSIX_SAVED_IDS) && _POSIX_SAVED_IDS > 0
	r = sysconf(_SC_SAVED_IDS);
	pc_check(r != -1, "_POSIX_SAVED_IDS is greater than zero but sysconf(_SC_SAVED_IDS) is -1");
#endif
#if defined(_POSIX_SEMAPHORES) && _POSIX_SEMAPHORES > 0
	r = sysconf(_SC_SEMAPHORES);
	pc_check(r != -1, "_POSIX_SEMAPHORES is greater than zero but sysconf(_SC_SEMAPHORES) is -1");
#endif
#if defined(_POSIX_SHARED_MEMORY_OBJECTS) && _POSIX_SHARED_MEMORY_OBJECTS > 0
	r = sysconf(_SC_SHARED_MEMORY_OBJECTS);
	pc_check(r != -1, "_POSIX_SHARED_MEMORY_OBJECTS is greater than zero but sysconf(_SC_SHARED_MEMORY_OBJECTS) is -1");
#endif
#if defined(_POSIX_SHELL) && _POSIX_SHELL > 0
	r = sysconf(_SC_SHELL);
	pc_check(r != -1, "_POSIX_SHELL is greater than zero but sysconf(_SC_SHELL) is -1");
#endif
#if defined(_POSIX_SPAWN) && _POSIX_SPAWN > 0
	r = sysconf(_SC_SPAWN);
	pc_check(r != -1, "_POSIX_SPAWN is greater than zero but sysconf(_SC_SPAWN) is -1");
#endif
#if defined(_POSIX_SPIN_LOCKS) && _POSIX_SPIN_LOCKS > 0
	r = sysconf(_SC_SPIN_LOCKS);
	pc_check(r != -1, "_POSIX_SPIN_LOCKS is greater than zero but sysconf(_SC_SPIN_LOCKS) is -1");
#endif
#if defined(_POSIX_SPORADIC_SERVER) && _POSIX_SPORADIC_SERVER > 0
	r = sysconf(_SC_SPORADIC_SERVER);
	pc_check(r != -1, "_POSIX_SPORADIC_SERVER is greater than zero but sysconf(_SC_SPORADIC_SERVER) is -1");
#endif
#if defined(_POSIX_SYNCHRONIZED_IO) && _POSIX_SYNCHRONIZED_IO > 0
	r = sysconf(_SC_SYNCHRONIZED_IO);
	pc_check(r != -1, "_POSIX_SYNCHRONIZED_IO is greater than zero but sysconf(_SC_SYNCHRONIZED_IO) is -1");
#endif
#if defined(_POSIX_THREADS) && _POSIX_THREADS > 0
	r = sysconf(_SC_THREADS);
	pc_check(r != -1, "_POSIX_THREADS is greater than zero but sysconf(_SC_THREADS) is -1");
#endif
#if defined(_POSIX_THREAD_ATTR_STACKADDR) && _POSIX_THREAD_ATTR_STACKADDR > 0
	r = sysconf(_SC_THREAD_ATTR_STACKADDR);
	pc_check(r != -1, "_POSIX_THREAD_ATTR_STACKADDR is greater than zero but sysconf(_SC_THREAD_ATTR_STACKADDR) is -1");
#endif
#if defined(_POSIX_THREAD_ATTR_STACKSIZE) && _POSIX_THREAD_ATTR_STACKSIZE > 0
	r = sysconf(_SC_THREAD_ATTR_STACKSIZE);
	pc_check(r != -1, "_POSIX_THREAD_ATTR_STACKSIZE is greater than zero but sysconf(_SC_THREAD_ATTR_STACKSIZE) is -1");
#endif
#if defined(_POSIX_THREAD_CPUTIME) && _POSIX_THREAD_CPUTIME > 0
	r = sysconf(_SC_THREAD_CPUTIME);
	pc_check(r != -1, "_POSIX_THREAD_CPUTIME is greater than zero but sysconf(_SC_THREAD_CPUTIME) is -1");
#endif
#if defined(_POSIX_THREAD_PRIORITY_SCHEDULING) && _POSIX_THREAD_PRIORITY_SCHEDULING > 0
	r = sysconf(_SC_THREAD_PRIORITY_SCHEDULING);
	pc_check(r != -1, "_POSIX_THREAD_PRIORITY_SCHEDULING is greater than zero but sysconf(_SC_THREAD_PRIORITY_SCHEDULING) is -1");
#endif
#if defined(_POSIX_THREAD_PRIO_INHERIT) && _POSIX_THREAD_PRIO_INHERIT > 0
	r = sysconf(_SC_THREAD_PRIO_INHERIT);
	pc_check(r != -1, "_POSIX_THREAD_PRIO_INHERIT is greater than zero but sysconf(_SC_THREAD_PRIO_INHERIT) is -1");
#endif
#if defined(_POSIX_THREAD_PRIO_PROTECT) && _POSIX_THREAD_PRIO_PROTECT > 0
	r = sysconf(_SC_THREAD_PRIO_PROTECT);
	pc_check(r != -1, "_POSIX_THREAD_PRIO_PROTECT is greater than zero but sysconf(_SC_THREAD_PRIO_PROTECT) is -1");
#endif
#if defined(_POSIX_THREAD_PROCESS_SHARED) && _POSIX_THREAD_PROCESS_SHARED > 0
	r = sysconf(_SC_THREAD_PROCESS_SHARED);
	pc_check(r != -1, "_POSIX_THREAD_PROCESS_SHARED is greater than zero but sysconf(_SC_THREAD_PROCESS_SHARED) is -1");
#endif
#if defined(_POSIX_THREAD_ROBUST_PRIO_INHERIT) && _POSIX_THREAD_ROBUST_PRIO_INHERIT > 0
	r = sysconf(_SC_THREAD_ROBUST_PRIO_INHERIT);
	pc_check(r != -1, "_POSIX_THREAD_ROBUST_PRIO_INHERIT is greater than zero but sysconf(_SC_THREAD_ROBUST_PRIO_INHERIT) is -1");
#endif
#if defined(_POSIX_THREAD_ROBUST_PRIO_PROTECT) && _POSIX_THREAD_ROBUST_PRIO_PROTECT > 0
	r = sysconf(_SC_THREAD_ROBUST_PRIO_PROTECT);
	pc_check(r != -1, "_POSIX_THREAD_ROBUST_PRIO_PROTECT is greater than zero but sysconf(_SC_THREAD_ROBUST_PRIO_PROTECT) is -1");
#endif
#if defined(_POSIX_THREAD_SAFE_FUNCTIONS) && _POSIX_THREAD_SAFE_FUNCTIONS > 0
	r = sysconf(_SC_THREAD_SAFE_FUNCTIONS);
	pc_check(r != -1, "_POSIX_THREAD_SAFE_FUNCTIONS is greater than zero but sysconf(_SC_THREAD_SAFE_FUNCTIONS) is -1");
#endif
#if defined(_POSIX_THREAD_SPORADIC_SERVER) && _POSIX_THREAD_SPORADIC_SERVER > 0
	r = sysconf(_SC_THREAD_SPORADIC_SERVER);
	pc_check(r != -1, "_POSIX_THREAD_SPORADIC_SERVER is greater than zero but sysconf(_SC_THREAD_SPORADIC_SERVER) is -1");
#endif
#if defined(_POSIX_TIMEOUTS) && _POSIX_TIMEOUTS > 0
	r = sysconf(_SC_TIMEOUTS);
	pc_check(r != -1, "_POSIX_TIMEOUTS is greater than zero but sysconf(_SC_TIMEOUTS) is -1");
#endif
#if defined(_POSIX_TIMERS) && _POSIX_TIMERS > 0
	r = sysconf(_SC_TIMERS);
	pc_check(r != -1, "_POSIX_TIMERS is greater than zero but sysconf(_SC_TIMERS) is -1");
#endif
#if defined(_POSIX_TYPED_MEMORY_OBJECTS) && _POSIX_TYPED_MEMORY_OBJECTS > 0
	r = sysconf(_SC_TYPED_MEMORY_OBJECTS);
	pc_check(r != -1, "_POSIX_TYPED_MEMORY_OBJECTS is greater than zero but sysconf(_SC_TYPED_MEMORY_OBJECTS) is -1");
#endif
#if defined(_POSIX_V7_ILP32_OFF32) && _POSIX_V7_ILP32_OFF32 > 0
	r = sysconf(_SC_V7_ILP32_OFF32);
	pc_check(r != -1, "_POSIX_V7_ILP32_OFF32 is greater than zero but sysconf(_SC_V7_ILP32_OFF32) is -1");
#endif
#if defined(_POSIX_V7_ILP32_OFFBIG) && _POSIX_V7_ILP32_OFFBIG > 0
	r = sysconf(_SC_V7_ILP32_OFFBIG);
	pc_check(r != -1, "_POSIX_V7_ILP32_OFFBIG is greater than zero but sysconf(_SC_V7_ILP32_OFFBIG) is -1");
#endif
#if defined(_POSIX_V7_LP64_OFF64) && _POSIX_V7_LP64_OFF64 > 0
	r = sysconf(_SC_V7_LP64_OFF64);
	pc_check(r != -1, "_POSIX_V7_LP64_OFF64 is greater than zero but sysconf(_SC_V7_LP64_OFF64) is -1");
#endif
#if defined(_POSIX_V7_LPBIG_OFFBIG) && _POSIX_V7_LPBIG_OFFBIG > 0
	r = sysconf(_SC_V7_LPBIG_OFFBIG);
	pc_check(r != -1, "_POSIX_V7_LPBIG_OFFBIG is greater than zero but sysconf(_SC_V7_LPBIG_OFFBIG) is -1");
#endif
#if defined(_POSIX_V8_ILP32_OFF32) && _POSIX_V8_ILP32_OFF32 > 0
	r = sysconf(_SC_V8_ILP32_OFF32);
	pc_check(r != -1, "_POSIX_V8_ILP32_OFF32 is greater than zero but sysconf(_SC_V8_ILP32_OFF32) is -1");
#endif
#if defined(_POSIX_V8_ILP32_OFFBIG) && _POSIX_V8_ILP32_OFFBIG > 0
	r = sysconf(_SC_V8_ILP32_OFFBIG);
	pc_check(r != -1, "_POSIX_V8_ILP32_OFFBIG is greater than zero but sysconf(_SC_V8_ILP32_OFFBIG) is -1");
#endif
#if defined(_POSIX_V8_LP64_OFF64) && _POSIX_V8_LP64_OFF64 > 0
	r = sysconf(_SC_V8_LP64_OFF64);
	pc_check(r != -1, "_POSIX_V8_LP64_OFF64 is greater than zero but sysconf(_SC_V8_LP64_OFF64) is -1");
#endif
#if defined(_POSIX_V8_LPBIG_OFFBIG) && _POSIX_V8_LPBIG_OFFBIG > 0
	r = sysconf(_SC_V8_LPBIG_OFFBIG);
	pc_check(r != -1, "_POSIX_V8_LPBIG_OFFBIG is greater than zero but sysconf(_SC_V8_LPBIG_OFFBIG) is -1");
#endif
#if defined(_XOPEN_CRYPT) && _XOPEN_CRYPT > 0
	r = sysconf(_SC_XOPEN_CRYPT);
	pc_check(r != -1, "_XOPEN_CRYPT is greater than zero but sysconf(_SC_XOPEN_CRYPT) is -1");
#endif
#if defined(_XOPEN_ENH_I18N) && _XOPEN_ENH_I18N > 0
	r = sysconf(_SC_XOPEN_ENH_I18N);
	pc_check(r != -1, "_XOPEN_ENH_I18N is greater than zero but sysconf(_SC_XOPEN_ENH_I18N) is -1");
#endif
#if defined(_XOPEN_REALTIME) && _XOPEN_REALTIME > 0
	r = sysconf(_SC_XOPEN_REALTIME);
	pc_check(r != -1, "_XOPEN_REALTIME is greater than zero but sysconf(_SC_XOPEN_REALTIME) is -1");
#endif
#if defined(_XOPEN_REALTIME_THREADS) && _XOPEN_REALTIME_THREADS > 0
	r = sysconf(_SC_XOPEN_REALTIME_THREADS);
	pc_check(r != -1, "_XOPEN_REALTIME_THREADS is greater than zero but sysconf(_SC_XOPEN_REALTIME_THREADS) is -1");
#endif
#if defined(_XOPEN_SHM) && _XOPEN_SHM > 0
	r = sysconf(_SC_XOPEN_SHM);
	pc_check(r != -1, "_XOPEN_SHM is greater than zero but sysconf(_SC_XOPEN_SHM) is -1");
#endif
#if defined(_XOPEN_UNIX) && _XOPEN_UNIX > 0
	r = sysconf(_SC_XOPEN_UNIX);
	pc_check(r != -1, "_XOPEN_UNIX is greater than zero but sysconf(_SC_XOPEN_UNIX) is -1");
#endif
#if defined(_XOPEN_UUCP) && _XOPEN_UUCP > 0
	r = sysconf(_SC_XOPEN_UUCP);
	pc_check(r != -1, "_XOPEN_UUCP is greater than zero but sysconf(_SC_XOPEN_UUCP) is -1");
#endif
	pc_pass();
}
