/*
 * No option symbol is below -1.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.unistd.h.009
 */
#include <unistd.h>
#include "pc.h"
/* The options symbols are all -1 or more when defined, and usable in #if. */
#if defined(_POSIX2_CHAR_TERM) && _POSIX2_CHAR_TERM < -1
#error "_POSIX2_CHAR_TERM is below -1"
#endif
#if defined(_POSIX2_C_BIND) && _POSIX2_C_BIND < -1
#error "_POSIX2_C_BIND is below -1"
#endif
#if defined(_POSIX2_C_DEV) && _POSIX2_C_DEV < -1
#error "_POSIX2_C_DEV is below -1"
#endif
#if defined(_POSIX2_FORT_RUN) && _POSIX2_FORT_RUN < -1
#error "_POSIX2_FORT_RUN is below -1"
#endif
#if defined(_POSIX2_LOCALEDEF) && _POSIX2_LOCALEDEF < -1
#error "_POSIX2_LOCALEDEF is below -1"
#endif
#if defined(_POSIX2_SW_DEV) && _POSIX2_SW_DEV < -1
#error "_POSIX2_SW_DEV is below -1"
#endif
#if defined(_POSIX2_UPE) && _POSIX2_UPE < -1
#error "_POSIX2_UPE is below -1"
#endif
#if defined(_POSIX_ADVISORY_INFO) && _POSIX_ADVISORY_INFO < -1
#error "_POSIX_ADVISORY_INFO is below -1"
#endif
#if defined(_POSIX_ASYNCHRONOUS_IO) && _POSIX_ASYNCHRONOUS_IO < -1
#error "_POSIX_ASYNCHRONOUS_IO is below -1"
#endif
#if defined(_POSIX_BARRIERS) && _POSIX_BARRIERS < -1
#error "_POSIX_BARRIERS is below -1"
#endif
#if defined(_POSIX_CHOWN_RESTRICTED) && _POSIX_CHOWN_RESTRICTED < -1
#error "_POSIX_CHOWN_RESTRICTED is below -1"
#endif
#if defined(_POSIX_CLOCK_SELECTION) && _POSIX_CLOCK_SELECTION < -1
#error "_POSIX_CLOCK_SELECTION is below -1"
#endif
#if defined(_POSIX_CPUTIME) && _POSIX_CPUTIME < -1
#error "_POSIX_CPUTIME is below -1"
#endif
#if defined(_POSIX_DEVICE_CONTROL) && _POSIX_DEVICE_CONTROL < -1
#error "_POSIX_DEVICE_CONTROL is below -1"
#endif
#if defined(_POSIX_FSYNC) && _POSIX_FSYNC < -1
#error "_POSIX_FSYNC is below -1"
#endif
#if defined(_POSIX_IPV6) && _POSIX_IPV6 < -1
#error "_POSIX_IPV6 is below -1"
#endif
#if defined(_POSIX_JOB_CONTROL) && _POSIX_JOB_CONTROL < -1
#error "_POSIX_JOB_CONTROL is below -1"
#endif
#if defined(_POSIX_MAPPED_FILES) && _POSIX_MAPPED_FILES < -1
#error "_POSIX_MAPPED_FILES is below -1"
#endif
#if defined(_POSIX_MEMLOCK) && _POSIX_MEMLOCK < -1
#error "_POSIX_MEMLOCK is below -1"
#endif
#if defined(_POSIX_MEMLOCK_RANGE) && _POSIX_MEMLOCK_RANGE < -1
#error "_POSIX_MEMLOCK_RANGE is below -1"
#endif
#if defined(_POSIX_MEMORY_PROTECTION) && _POSIX_MEMORY_PROTECTION < -1
#error "_POSIX_MEMORY_PROTECTION is below -1"
#endif
#if defined(_POSIX_MESSAGE_PASSING) && _POSIX_MESSAGE_PASSING < -1
#error "_POSIX_MESSAGE_PASSING is below -1"
#endif
#if defined(_POSIX_MONOTONIC_CLOCK) && _POSIX_MONOTONIC_CLOCK < -1
#error "_POSIX_MONOTONIC_CLOCK is below -1"
#endif
#if defined(_POSIX_NO_TRUNC) && _POSIX_NO_TRUNC < -1
#error "_POSIX_NO_TRUNC is below -1"
#endif
#if defined(_POSIX_PRIORITIZED_IO) && _POSIX_PRIORITIZED_IO < -1
#error "_POSIX_PRIORITIZED_IO is below -1"
#endif
#if defined(_POSIX_PRIORITY_SCHEDULING) && _POSIX_PRIORITY_SCHEDULING < -1
#error "_POSIX_PRIORITY_SCHEDULING is below -1"
#endif
#if defined(_POSIX_RAW_SOCKETS) && _POSIX_RAW_SOCKETS < -1
#error "_POSIX_RAW_SOCKETS is below -1"
#endif
#if defined(_POSIX_READER_WRITER_LOCKS) && _POSIX_READER_WRITER_LOCKS < -1
#error "_POSIX_READER_WRITER_LOCKS is below -1"
#endif
#if defined(_POSIX_REALTIME_SIGNALS) && _POSIX_REALTIME_SIGNALS < -1
#error "_POSIX_REALTIME_SIGNALS is below -1"
#endif
#if defined(_POSIX_REGEXP) && _POSIX_REGEXP < -1
#error "_POSIX_REGEXP is below -1"
#endif
#if defined(_POSIX_SAVED_IDS) && _POSIX_SAVED_IDS < -1
#error "_POSIX_SAVED_IDS is below -1"
#endif
#if defined(_POSIX_SEMAPHORES) && _POSIX_SEMAPHORES < -1
#error "_POSIX_SEMAPHORES is below -1"
#endif
#if defined(_POSIX_SHARED_MEMORY_OBJECTS) && _POSIX_SHARED_MEMORY_OBJECTS < -1
#error "_POSIX_SHARED_MEMORY_OBJECTS is below -1"
#endif
#if defined(_POSIX_SHELL) && _POSIX_SHELL < -1
#error "_POSIX_SHELL is below -1"
#endif
#if defined(_POSIX_SPAWN) && _POSIX_SPAWN < -1
#error "_POSIX_SPAWN is below -1"
#endif
#if defined(_POSIX_SPIN_LOCKS) && _POSIX_SPIN_LOCKS < -1
#error "_POSIX_SPIN_LOCKS is below -1"
#endif
#if defined(_POSIX_SPORADIC_SERVER) && _POSIX_SPORADIC_SERVER < -1
#error "_POSIX_SPORADIC_SERVER is below -1"
#endif
#if defined(_POSIX_SYNCHRONIZED_IO) && _POSIX_SYNCHRONIZED_IO < -1
#error "_POSIX_SYNCHRONIZED_IO is below -1"
#endif
#if defined(_POSIX_THREADS) && _POSIX_THREADS < -1
#error "_POSIX_THREADS is below -1"
#endif
#if defined(_POSIX_THREAD_ATTR_STACKADDR) && _POSIX_THREAD_ATTR_STACKADDR < -1
#error "_POSIX_THREAD_ATTR_STACKADDR is below -1"
#endif
#if defined(_POSIX_THREAD_ATTR_STACKSIZE) && _POSIX_THREAD_ATTR_STACKSIZE < -1
#error "_POSIX_THREAD_ATTR_STACKSIZE is below -1"
#endif
#if defined(_POSIX_THREAD_CPUTIME) && _POSIX_THREAD_CPUTIME < -1
#error "_POSIX_THREAD_CPUTIME is below -1"
#endif
#if defined(_POSIX_THREAD_PRIORITY_SCHEDULING) && _POSIX_THREAD_PRIORITY_SCHEDULING < -1
#error "_POSIX_THREAD_PRIORITY_SCHEDULING is below -1"
#endif
#if defined(_POSIX_THREAD_PRIO_INHERIT) && _POSIX_THREAD_PRIO_INHERIT < -1
#error "_POSIX_THREAD_PRIO_INHERIT is below -1"
#endif
#if defined(_POSIX_THREAD_PRIO_PROTECT) && _POSIX_THREAD_PRIO_PROTECT < -1
#error "_POSIX_THREAD_PRIO_PROTECT is below -1"
#endif
#if defined(_POSIX_THREAD_PROCESS_SHARED) && _POSIX_THREAD_PROCESS_SHARED < -1
#error "_POSIX_THREAD_PROCESS_SHARED is below -1"
#endif
#if defined(_POSIX_THREAD_ROBUST_PRIO_INHERIT) && _POSIX_THREAD_ROBUST_PRIO_INHERIT < -1
#error "_POSIX_THREAD_ROBUST_PRIO_INHERIT is below -1"
#endif
#if defined(_POSIX_THREAD_ROBUST_PRIO_PROTECT) && _POSIX_THREAD_ROBUST_PRIO_PROTECT < -1
#error "_POSIX_THREAD_ROBUST_PRIO_PROTECT is below -1"
#endif
#if defined(_POSIX_THREAD_SAFE_FUNCTIONS) && _POSIX_THREAD_SAFE_FUNCTIONS < -1
#error "_POSIX_THREAD_SAFE_FUNCTIONS is below -1"
#endif
#if defined(_POSIX_THREAD_SPORADIC_SERVER) && _POSIX_THREAD_SPORADIC_SERVER < -1
#error "_POSIX_THREAD_SPORADIC_SERVER is below -1"
#endif
#if defined(_POSIX_TIMEOUTS) && _POSIX_TIMEOUTS < -1
#error "_POSIX_TIMEOUTS is below -1"
#endif
#if defined(_POSIX_TIMERS) && _POSIX_TIMERS < -1
#error "_POSIX_TIMERS is below -1"
#endif
#if defined(_POSIX_TYPED_MEMORY_OBJECTS) && _POSIX_TYPED_MEMORY_OBJECTS < -1
#error "_POSIX_TYPED_MEMORY_OBJECTS is below -1"
#endif
#if defined(_POSIX_V7_ILP32_OFF32) && _POSIX_V7_ILP32_OFF32 < -1
#error "_POSIX_V7_ILP32_OFF32 is below -1"
#endif
#if defined(_POSIX_V7_ILP32_OFFBIG) && _POSIX_V7_ILP32_OFFBIG < -1
#error "_POSIX_V7_ILP32_OFFBIG is below -1"
#endif
#if defined(_POSIX_V7_LP64_OFF64) && _POSIX_V7_LP64_OFF64 < -1
#error "_POSIX_V7_LP64_OFF64 is below -1"
#endif
#if defined(_POSIX_V7_LPBIG_OFFBIG) && _POSIX_V7_LPBIG_OFFBIG < -1
#error "_POSIX_V7_LPBIG_OFFBIG is below -1"
#endif
#if defined(_POSIX_V8_ILP32_OFF32) && _POSIX_V8_ILP32_OFF32 < -1
#error "_POSIX_V8_ILP32_OFF32 is below -1"
#endif
#if defined(_POSIX_V8_ILP32_OFFBIG) && _POSIX_V8_ILP32_OFFBIG < -1
#error "_POSIX_V8_ILP32_OFFBIG is below -1"
#endif
#if defined(_POSIX_V8_LP64_OFF64) && _POSIX_V8_LP64_OFF64 < -1
#error "_POSIX_V8_LP64_OFF64 is below -1"
#endif
#if defined(_POSIX_V8_LPBIG_OFFBIG) && _POSIX_V8_LPBIG_OFFBIG < -1
#error "_POSIX_V8_LPBIG_OFFBIG is below -1"
#endif
#if defined(_XOPEN_CRYPT) && _XOPEN_CRYPT < -1
#error "_XOPEN_CRYPT is below -1"
#endif
#if defined(_XOPEN_ENH_I18N) && _XOPEN_ENH_I18N < -1
#error "_XOPEN_ENH_I18N is below -1"
#endif
#if defined(_XOPEN_REALTIME) && _XOPEN_REALTIME < -1
#error "_XOPEN_REALTIME is below -1"
#endif
#if defined(_XOPEN_REALTIME_THREADS) && _XOPEN_REALTIME_THREADS < -1
#error "_XOPEN_REALTIME_THREADS is below -1"
#endif
#if defined(_XOPEN_SHM) && _XOPEN_SHM < -1
#error "_XOPEN_SHM is below -1"
#endif
#if defined(_XOPEN_UNIX) && _XOPEN_UNIX < -1
#error "_XOPEN_UNIX is below -1"
#endif
#if defined(_XOPEN_UUCP) && _XOPEN_UUCP < -1
#error "_XOPEN_UUCP is below -1"
#endif

int main(void)
{
	pc_begin("XBD.unistd.h.009");
	pc_test();
	pc_pass();
}
