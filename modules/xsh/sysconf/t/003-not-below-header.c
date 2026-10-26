/*
 * sysconf() limits are no more restrictive than the <limits.h> values.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sysconf.003
 * MUTANT    sysconf.ngroups_min
 * MUTANT    sysconf.stack_min_doubled
 * VARIANT   sysconf.no_limit
 * VARIANT   sysconf.minimal
 * VARIANT   sysconf.errno_on_success
 */
#include "pc.h"
#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <unistd.h>

/* The names of the sysconf() table (kind, minimum, option needed). */
struct var {
	const char *name;
	int sc;
	char kind;	/* see below */
	long min;
	const char *dep;	/* option the variable depends on, or NULL */
};

/*
 * kinds: L a <limits.h> maximum or minimum (-1: no limit), S a size (at
 * least min), A an option always 202405L, P an option always greater
 * than zero, Q an option sysconf() reports as -1 or 202405L, T one it
 * reports as -1 or greater than zero, N another option, V a version,
 * X another variable.
 */
static const struct var vars[] = {
#ifdef _SC_AIO_LISTIO_MAX
	{ "_SC_AIO_LISTIO_MAX", _SC_AIO_LISTIO_MAX, 'L', 2, NULL },
#endif
#ifdef _SC_AIO_MAX
	{ "_SC_AIO_MAX", _SC_AIO_MAX, 'L', 1, NULL },
#endif
#ifdef _SC_AIO_PRIO_DELTA_MAX
	{ "_SC_AIO_PRIO_DELTA_MAX", _SC_AIO_PRIO_DELTA_MAX, 'L', 0, NULL },
#endif
#ifdef _SC_ARG_MAX
	{ "_SC_ARG_MAX", _SC_ARG_MAX, 'L', 4096, NULL },
#endif
#ifdef _SC_ATEXIT_MAX
	{ "_SC_ATEXIT_MAX", _SC_ATEXIT_MAX, 'L', 32, NULL },
#endif
#ifdef _SC_BC_BASE_MAX
	{ "_SC_BC_BASE_MAX", _SC_BC_BASE_MAX, 'L', 99, NULL },
#endif
#ifdef _SC_BC_DIM_MAX
	{ "_SC_BC_DIM_MAX", _SC_BC_DIM_MAX, 'L', 2048, NULL },
#endif
#ifdef _SC_BC_SCALE_MAX
	{ "_SC_BC_SCALE_MAX", _SC_BC_SCALE_MAX, 'L', 99, NULL },
#endif
#ifdef _SC_BC_STRING_MAX
	{ "_SC_BC_STRING_MAX", _SC_BC_STRING_MAX, 'L', 1000, NULL },
#endif
#ifdef _SC_CHILD_MAX
	{ "_SC_CHILD_MAX", _SC_CHILD_MAX, 'L', 25, NULL },
#endif
#ifdef _SC_CLK_TCK
	{ "_SC_CLK_TCK", _SC_CLK_TCK, 'X', 0, NULL },
#endif
#ifdef _SC_COLL_WEIGHTS_MAX
	{ "_SC_COLL_WEIGHTS_MAX", _SC_COLL_WEIGHTS_MAX, 'L', 2, NULL },
#endif
#ifdef _SC_DELAYTIMER_MAX
	{ "_SC_DELAYTIMER_MAX", _SC_DELAYTIMER_MAX, 'L', 32, NULL },
#endif
#ifdef _SC_EXPR_NEST_MAX
	{ "_SC_EXPR_NEST_MAX", _SC_EXPR_NEST_MAX, 'L', 32, NULL },
#endif
#ifdef _SC_HOST_NAME_MAX
	{ "_SC_HOST_NAME_MAX", _SC_HOST_NAME_MAX, 'L', 255, NULL },
#endif
#ifdef _SC_IOV_MAX
	{ "_SC_IOV_MAX", _SC_IOV_MAX, 'L', 16, "XSI" },
#endif
#ifdef _SC_LINE_MAX
	{ "_SC_LINE_MAX", _SC_LINE_MAX, 'L', 2048, NULL },
#endif
#ifdef _SC_LOGIN_NAME_MAX
	{ "_SC_LOGIN_NAME_MAX", _SC_LOGIN_NAME_MAX, 'L', 9, NULL },
#endif
#ifdef _SC_NGROUPS_MAX
	{ "_SC_NGROUPS_MAX", _SC_NGROUPS_MAX, 'L', 8, NULL },
#endif
#ifdef _SC_GETGR_R_SIZE_MAX
	{ "_SC_GETGR_R_SIZE_MAX", _SC_GETGR_R_SIZE_MAX, 'X', 0, NULL },
#endif
#ifdef _SC_GETPW_R_SIZE_MAX
	{ "_SC_GETPW_R_SIZE_MAX", _SC_GETPW_R_SIZE_MAX, 'X', 0, NULL },
#endif
#ifdef _SC_MQ_OPEN_MAX
	{ "_SC_MQ_OPEN_MAX", _SC_MQ_OPEN_MAX, 'L', 8, "MSG" },
#endif
#ifdef _SC_MQ_PRIO_MAX
	{ "_SC_MQ_PRIO_MAX", _SC_MQ_PRIO_MAX, 'L', 32, "MSG" },
#endif
#ifdef _SC_NPROCESSORS_CONF
	{ "_SC_NPROCESSORS_CONF", _SC_NPROCESSORS_CONF, 'X', 0, NULL },
#endif
#ifdef _SC_NPROCESSORS_ONLN
	{ "_SC_NPROCESSORS_ONLN", _SC_NPROCESSORS_ONLN, 'X', 0, NULL },
#endif
#ifdef _SC_NSIG
	{ "_SC_NSIG", _SC_NSIG, 'X', 0, NULL },
#endif
#ifdef _SC_OPEN_MAX
	{ "_SC_OPEN_MAX", _SC_OPEN_MAX, 'L', 20, NULL },
#endif
#ifdef _SC_PAGE_SIZE
	{ "_SC_PAGE_SIZE", _SC_PAGE_SIZE, 'S', 1, "XSI" },
#endif
#ifdef _SC_PAGESIZE
	{ "_SC_PAGESIZE", _SC_PAGESIZE, 'S', 1, NULL },
#endif
#ifdef _SC_THREAD_DESTRUCTOR_ITERATIONS
	{ "_SC_THREAD_DESTRUCTOR_ITERATIONS", _SC_THREAD_DESTRUCTOR_ITERATIONS, 'L', 4, NULL },
#endif
#ifdef _SC_THREAD_KEYS_MAX
	{ "_SC_THREAD_KEYS_MAX", _SC_THREAD_KEYS_MAX, 'L', 128, NULL },
#endif
#ifdef _SC_THREAD_STACK_MIN
	{ "_SC_THREAD_STACK_MIN", _SC_THREAD_STACK_MIN, 'L', 0, NULL },
#endif
#ifdef _SC_THREAD_THREADS_MAX
	{ "_SC_THREAD_THREADS_MAX", _SC_THREAD_THREADS_MAX, 'L', 64, NULL },
#endif
#ifdef _SC_RE_DUP_MAX
	{ "_SC_RE_DUP_MAX", _SC_RE_DUP_MAX, 'L', 255, NULL },
#endif
#ifdef _SC_RTSIG_MAX
	{ "_SC_RTSIG_MAX", _SC_RTSIG_MAX, 'L', 8, NULL },
#endif
#ifdef _SC_SEM_NSEMS_MAX
	{ "_SC_SEM_NSEMS_MAX", _SC_SEM_NSEMS_MAX, 'L', 256, NULL },
#endif
#ifdef _SC_SEM_VALUE_MAX
	{ "_SC_SEM_VALUE_MAX", _SC_SEM_VALUE_MAX, 'L', 32767, NULL },
#endif
#ifdef _SC_SIGQUEUE_MAX
	{ "_SC_SIGQUEUE_MAX", _SC_SIGQUEUE_MAX, 'L', 32, NULL },
#endif
#ifdef _SC_STREAM_MAX
	{ "_SC_STREAM_MAX", _SC_STREAM_MAX, 'L', 8, NULL },
#endif
#ifdef _SC_SYMLOOP_MAX
	{ "_SC_SYMLOOP_MAX", _SC_SYMLOOP_MAX, 'L', 8, NULL },
#endif
#ifdef _SC_TIMER_MAX
	{ "_SC_TIMER_MAX", _SC_TIMER_MAX, 'L', 32, NULL },
#endif
#ifdef _SC_TTY_NAME_MAX
	{ "_SC_TTY_NAME_MAX", _SC_TTY_NAME_MAX, 'L', 9, NULL },
#endif
#ifdef _SC_TZNAME_MAX
	{ "_SC_TZNAME_MAX", _SC_TZNAME_MAX, 'L', 6, NULL },
#endif
#ifdef _SC_ADVISORY_INFO
	{ "_SC_ADVISORY_INFO", _SC_ADVISORY_INFO, 'Q', 0, NULL },
#endif
#ifdef _SC_BARRIERS
	{ "_SC_BARRIERS", _SC_BARRIERS, 'A', 0, NULL },
#endif
#ifdef _SC_ASYNCHRONOUS_IO
	{ "_SC_ASYNCHRONOUS_IO", _SC_ASYNCHRONOUS_IO, 'A', 0, NULL },
#endif
#ifdef _SC_CLOCK_SELECTION
	{ "_SC_CLOCK_SELECTION", _SC_CLOCK_SELECTION, 'A', 0, NULL },
#endif
#ifdef _SC_CPUTIME
	{ "_SC_CPUTIME", _SC_CPUTIME, 'Q', 0, NULL },
#endif
#ifdef _SC_DEVICE_CONTROL
	{ "_SC_DEVICE_CONTROL", _SC_DEVICE_CONTROL, 'Q', 0, NULL },
#endif
#ifdef _SC_FSYNC
	{ "_SC_FSYNC", _SC_FSYNC, 'Q', 0, NULL },
#endif
#ifdef _SC_IPV6
	{ "_SC_IPV6", _SC_IPV6, 'Q', 0, NULL },
#endif
#ifdef _SC_JOB_CONTROL
	{ "_SC_JOB_CONTROL", _SC_JOB_CONTROL, 'P', 0, NULL },
#endif
#ifdef _SC_MAPPED_FILES
	{ "_SC_MAPPED_FILES", _SC_MAPPED_FILES, 'A', 0, NULL },
#endif
#ifdef _SC_MEMLOCK
	{ "_SC_MEMLOCK", _SC_MEMLOCK, 'Q', 0, NULL },
#endif
#ifdef _SC_MEMLOCK_RANGE
	{ "_SC_MEMLOCK_RANGE", _SC_MEMLOCK_RANGE, 'Q', 0, NULL },
#endif
#ifdef _SC_MEMORY_PROTECTION
	{ "_SC_MEMORY_PROTECTION", _SC_MEMORY_PROTECTION, 'A', 0, NULL },
#endif
#ifdef _SC_MESSAGE_PASSING
	{ "_SC_MESSAGE_PASSING", _SC_MESSAGE_PASSING, 'Q', 0, NULL },
#endif
#ifdef _SC_MONOTONIC_CLOCK
	{ "_SC_MONOTONIC_CLOCK", _SC_MONOTONIC_CLOCK, 'A', 0, NULL },
#endif
#ifdef _SC_PRIORITIZED_IO
	{ "_SC_PRIORITIZED_IO", _SC_PRIORITIZED_IO, 'Q', 0, NULL },
#endif
#ifdef _SC_PRIORITY_SCHEDULING
	{ "_SC_PRIORITY_SCHEDULING", _SC_PRIORITY_SCHEDULING, 'Q', 0, NULL },
#endif
#ifdef _SC_RAW_SOCKETS
	{ "_SC_RAW_SOCKETS", _SC_RAW_SOCKETS, 'Q', 0, NULL },
#endif
#ifdef _SC_READER_WRITER_LOCKS
	{ "_SC_READER_WRITER_LOCKS", _SC_READER_WRITER_LOCKS, 'A', 0, NULL },
#endif
#ifdef _SC_REALTIME_SIGNALS
	{ "_SC_REALTIME_SIGNALS", _SC_REALTIME_SIGNALS, 'A', 0, NULL },
#endif
#ifdef _SC_REGEXP
	{ "_SC_REGEXP", _SC_REGEXP, 'P', 0, NULL },
#endif
#ifdef _SC_SAVED_IDS
	{ "_SC_SAVED_IDS", _SC_SAVED_IDS, 'P', 0, NULL },
#endif
#ifdef _SC_SEMAPHORES
	{ "_SC_SEMAPHORES", _SC_SEMAPHORES, 'A', 0, NULL },
#endif
#ifdef _SC_SHARED_MEMORY_OBJECTS
	{ "_SC_SHARED_MEMORY_OBJECTS", _SC_SHARED_MEMORY_OBJECTS, 'Q', 0, NULL },
#endif
#ifdef _SC_SHELL
	{ "_SC_SHELL", _SC_SHELL, 'P', 0, NULL },
#endif
#ifdef _SC_SPAWN
	{ "_SC_SPAWN", _SC_SPAWN, 'Q', 0, NULL },
#endif
#ifdef _SC_SPIN_LOCKS
	{ "_SC_SPIN_LOCKS", _SC_SPIN_LOCKS, 'A', 0, NULL },
#endif
#ifdef _SC_SPORADIC_SERVER
	{ "_SC_SPORADIC_SERVER", _SC_SPORADIC_SERVER, 'Q', 0, NULL },
#endif
#ifdef _SC_SS_REPL_MAX
	{ "_SC_SS_REPL_MAX", _SC_SS_REPL_MAX, 'L', 4, "SS|TSP" },
#endif
#ifdef _SC_SYNCHRONIZED_IO
	{ "_SC_SYNCHRONIZED_IO", _SC_SYNCHRONIZED_IO, 'Q', 0, NULL },
#endif
#ifdef _SC_THREAD_ATTR_STACKADDR
	{ "_SC_THREAD_ATTR_STACKADDR", _SC_THREAD_ATTR_STACKADDR, 'Q', 0, NULL },
#endif
#ifdef _SC_THREAD_ATTR_STACKSIZE
	{ "_SC_THREAD_ATTR_STACKSIZE", _SC_THREAD_ATTR_STACKSIZE, 'Q', 0, NULL },
#endif
#ifdef _SC_THREAD_CPUTIME
	{ "_SC_THREAD_CPUTIME", _SC_THREAD_CPUTIME, 'Q', 0, NULL },
#endif
#ifdef _SC_THREAD_PRIO_INHERIT
	{ "_SC_THREAD_PRIO_INHERIT", _SC_THREAD_PRIO_INHERIT, 'Q', 0, NULL },
#endif
#ifdef _SC_THREAD_PRIO_PROTECT
	{ "_SC_THREAD_PRIO_PROTECT", _SC_THREAD_PRIO_PROTECT, 'Q', 0, NULL },
#endif
#ifdef _SC_THREAD_PRIORITY_SCHEDULING
	{ "_SC_THREAD_PRIORITY_SCHEDULING", _SC_THREAD_PRIORITY_SCHEDULING, 'Q', 0, NULL },
#endif
#ifdef _SC_THREAD_PROCESS_SHARED
	{ "_SC_THREAD_PROCESS_SHARED", _SC_THREAD_PROCESS_SHARED, 'Q', 0, NULL },
#endif
#ifdef _SC_THREAD_ROBUST_PRIO_INHERIT
	{ "_SC_THREAD_ROBUST_PRIO_INHERIT", _SC_THREAD_ROBUST_PRIO_INHERIT, 'Q', 0, NULL },
#endif
#ifdef _SC_THREAD_ROBUST_PRIO_PROTECT
	{ "_SC_THREAD_ROBUST_PRIO_PROTECT", _SC_THREAD_ROBUST_PRIO_PROTECT, 'Q', 0, NULL },
#endif
#ifdef _SC_THREAD_SAFE_FUNCTIONS
	{ "_SC_THREAD_SAFE_FUNCTIONS", _SC_THREAD_SAFE_FUNCTIONS, 'A', 0, NULL },
#endif
#ifdef _SC_THREAD_SPORADIC_SERVER
	{ "_SC_THREAD_SPORADIC_SERVER", _SC_THREAD_SPORADIC_SERVER, 'Q', 0, NULL },
#endif
#ifdef _SC_THREADS
	{ "_SC_THREADS", _SC_THREADS, 'A', 0, NULL },
#endif
#ifdef _SC_TIMEOUTS
	{ "_SC_TIMEOUTS", _SC_TIMEOUTS, 'A', 0, NULL },
#endif
#ifdef _SC_TIMERS
	{ "_SC_TIMERS", _SC_TIMERS, 'A', 0, NULL },
#endif
#ifdef _SC_TYPED_MEMORY_OBJECTS
	{ "_SC_TYPED_MEMORY_OBJECTS", _SC_TYPED_MEMORY_OBJECTS, 'Q', 0, NULL },
#endif
#ifdef _SC_VERSION
	{ "_SC_VERSION", _SC_VERSION, 'V', 0, NULL },
#endif
#ifdef _SC_V8_ILP32_OFF32
	{ "_SC_V8_ILP32_OFF32", _SC_V8_ILP32_OFF32, 'N', 0, NULL },
#endif
#ifdef _SC_V8_ILP32_OFFBIG
	{ "_SC_V8_ILP32_OFFBIG", _SC_V8_ILP32_OFFBIG, 'N', 0, NULL },
#endif
#ifdef _SC_V8_LP64_OFF64
	{ "_SC_V8_LP64_OFF64", _SC_V8_LP64_OFF64, 'N', 0, NULL },
#endif
#ifdef _SC_V8_LPBIG_OFFBIG
	{ "_SC_V8_LPBIG_OFFBIG", _SC_V8_LPBIG_OFFBIG, 'N', 0, NULL },
#endif
#ifdef _SC_V7_ILP32_OFF32
	{ "_SC_V7_ILP32_OFF32", _SC_V7_ILP32_OFF32, 'N', 0, NULL },
#endif
#ifdef _SC_V7_ILP32_OFFBIG
	{ "_SC_V7_ILP32_OFFBIG", _SC_V7_ILP32_OFFBIG, 'N', 0, NULL },
#endif
#ifdef _SC_V7_LP64_OFF64
	{ "_SC_V7_LP64_OFF64", _SC_V7_LP64_OFF64, 'N', 0, NULL },
#endif
#ifdef _SC_V7_LPBIG_OFFBIG
	{ "_SC_V7_LPBIG_OFFBIG", _SC_V7_LPBIG_OFFBIG, 'N', 0, NULL },
#endif
#ifdef _SC_2_C_BIND
	{ "_SC_2_C_BIND", _SC_2_C_BIND, 'A', 0, NULL },
#endif
#ifdef _SC_2_C_DEV
	{ "_SC_2_C_DEV", _SC_2_C_DEV, 'Q', 0, NULL },
#endif
#ifdef _SC_2_CHAR_TERM
	{ "_SC_2_CHAR_TERM", _SC_2_CHAR_TERM, 'T', 0, NULL },
#endif
#ifdef _SC_2_FORT_RUN
	{ "_SC_2_FORT_RUN", _SC_2_FORT_RUN, 'Q', 0, NULL },
#endif
#ifdef _SC_2_LOCALEDEF
	{ "_SC_2_LOCALEDEF", _SC_2_LOCALEDEF, 'Q', 0, NULL },
#endif
#ifdef _SC_2_SW_DEV
	{ "_SC_2_SW_DEV", _SC_2_SW_DEV, 'Q', 0, NULL },
#endif
#ifdef _SC_2_UPE
	{ "_SC_2_UPE", _SC_2_UPE, 'Q', 0, NULL },
#endif
#ifdef _SC_2_VERSION
	{ "_SC_2_VERSION", _SC_2_VERSION, 'V', 0, NULL },
#endif
#ifdef _SC_XOPEN_CRYPT
	{ "_SC_XOPEN_CRYPT", _SC_XOPEN_CRYPT, 'N', 0, "XSI" },
#endif
#ifdef _SC_XOPEN_ENH_I18N
	{ "_SC_XOPEN_ENH_I18N", _SC_XOPEN_ENH_I18N, 'N', 0, NULL },
#endif
#ifdef _SC_XOPEN_REALTIME
	{ "_SC_XOPEN_REALTIME", _SC_XOPEN_REALTIME, 'N', 0, NULL },
#endif
#ifdef _SC_XOPEN_REALTIME_THREADS
	{ "_SC_XOPEN_REALTIME_THREADS", _SC_XOPEN_REALTIME_THREADS, 'N', 0, NULL },
#endif
#ifdef _SC_XOPEN_SHM
	{ "_SC_XOPEN_SHM", _SC_XOPEN_SHM, 'N', 0, NULL },
#endif
#ifdef _SC_XOPEN_UNIX
	{ "_SC_XOPEN_UNIX", _SC_XOPEN_UNIX, 'N', 0, NULL },
#endif
#ifdef _SC_XOPEN_UUCP
	{ "_SC_XOPEN_UUCP", _SC_XOPEN_UUCP, 'Q', 0, NULL },
#endif
#ifdef _SC_XOPEN_VERSION
	{ "_SC_XOPEN_VERSION", _SC_XOPEN_VERSION, 'V', 0, "XSI" },
#endif
};

/* Names of the table <unistd.h> does not define. */
static const char *const missing[] = {
#ifndef _SC_AIO_LISTIO_MAX
	"_SC_AIO_LISTIO_MAX",
#endif
#ifndef _SC_AIO_MAX
	"_SC_AIO_MAX",
#endif
#ifndef _SC_AIO_PRIO_DELTA_MAX
	"_SC_AIO_PRIO_DELTA_MAX",
#endif
#ifndef _SC_ARG_MAX
	"_SC_ARG_MAX",
#endif
#ifndef _SC_ATEXIT_MAX
	"_SC_ATEXIT_MAX",
#endif
#ifndef _SC_BC_BASE_MAX
	"_SC_BC_BASE_MAX",
#endif
#ifndef _SC_BC_DIM_MAX
	"_SC_BC_DIM_MAX",
#endif
#ifndef _SC_BC_SCALE_MAX
	"_SC_BC_SCALE_MAX",
#endif
#ifndef _SC_BC_STRING_MAX
	"_SC_BC_STRING_MAX",
#endif
#ifndef _SC_CHILD_MAX
	"_SC_CHILD_MAX",
#endif
#ifndef _SC_CLK_TCK
	"_SC_CLK_TCK",
#endif
#ifndef _SC_COLL_WEIGHTS_MAX
	"_SC_COLL_WEIGHTS_MAX",
#endif
#ifndef _SC_DELAYTIMER_MAX
	"_SC_DELAYTIMER_MAX",
#endif
#ifndef _SC_EXPR_NEST_MAX
	"_SC_EXPR_NEST_MAX",
#endif
#ifndef _SC_HOST_NAME_MAX
	"_SC_HOST_NAME_MAX",
#endif
#ifndef _SC_IOV_MAX
	"_SC_IOV_MAX",
#endif
#ifndef _SC_LINE_MAX
	"_SC_LINE_MAX",
#endif
#ifndef _SC_LOGIN_NAME_MAX
	"_SC_LOGIN_NAME_MAX",
#endif
#ifndef _SC_NGROUPS_MAX
	"_SC_NGROUPS_MAX",
#endif
#ifndef _SC_GETGR_R_SIZE_MAX
	"_SC_GETGR_R_SIZE_MAX",
#endif
#ifndef _SC_GETPW_R_SIZE_MAX
	"_SC_GETPW_R_SIZE_MAX",
#endif
#ifndef _SC_MQ_OPEN_MAX
	"_SC_MQ_OPEN_MAX",
#endif
#ifndef _SC_MQ_PRIO_MAX
	"_SC_MQ_PRIO_MAX",
#endif
#ifndef _SC_NPROCESSORS_CONF
	"_SC_NPROCESSORS_CONF",
#endif
#ifndef _SC_NPROCESSORS_ONLN
	"_SC_NPROCESSORS_ONLN",
#endif
#ifndef _SC_NSIG
	"_SC_NSIG",
#endif
#ifndef _SC_OPEN_MAX
	"_SC_OPEN_MAX",
#endif
#ifndef _SC_PAGE_SIZE
	"_SC_PAGE_SIZE",
#endif
#ifndef _SC_PAGESIZE
	"_SC_PAGESIZE",
#endif
#ifndef _SC_THREAD_DESTRUCTOR_ITERATIONS
	"_SC_THREAD_DESTRUCTOR_ITERATIONS",
#endif
#ifndef _SC_THREAD_KEYS_MAX
	"_SC_THREAD_KEYS_MAX",
#endif
#ifndef _SC_THREAD_STACK_MIN
	"_SC_THREAD_STACK_MIN",
#endif
#ifndef _SC_THREAD_THREADS_MAX
	"_SC_THREAD_THREADS_MAX",
#endif
#ifndef _SC_RE_DUP_MAX
	"_SC_RE_DUP_MAX",
#endif
#ifndef _SC_RTSIG_MAX
	"_SC_RTSIG_MAX",
#endif
#ifndef _SC_SEM_NSEMS_MAX
	"_SC_SEM_NSEMS_MAX",
#endif
#ifndef _SC_SEM_VALUE_MAX
	"_SC_SEM_VALUE_MAX",
#endif
#ifndef _SC_SIGQUEUE_MAX
	"_SC_SIGQUEUE_MAX",
#endif
#ifndef _SC_STREAM_MAX
	"_SC_STREAM_MAX",
#endif
#ifndef _SC_SYMLOOP_MAX
	"_SC_SYMLOOP_MAX",
#endif
#ifndef _SC_TIMER_MAX
	"_SC_TIMER_MAX",
#endif
#ifndef _SC_TTY_NAME_MAX
	"_SC_TTY_NAME_MAX",
#endif
#ifndef _SC_TZNAME_MAX
	"_SC_TZNAME_MAX",
#endif
#ifndef _SC_ADVISORY_INFO
	"_SC_ADVISORY_INFO",
#endif
#ifndef _SC_BARRIERS
	"_SC_BARRIERS",
#endif
#ifndef _SC_ASYNCHRONOUS_IO
	"_SC_ASYNCHRONOUS_IO",
#endif
#ifndef _SC_CLOCK_SELECTION
	"_SC_CLOCK_SELECTION",
#endif
#ifndef _SC_CPUTIME
	"_SC_CPUTIME",
#endif
#ifndef _SC_DEVICE_CONTROL
	"_SC_DEVICE_CONTROL",
#endif
#ifndef _SC_FSYNC
	"_SC_FSYNC",
#endif
#ifndef _SC_IPV6
	"_SC_IPV6",
#endif
#ifndef _SC_JOB_CONTROL
	"_SC_JOB_CONTROL",
#endif
#ifndef _SC_MAPPED_FILES
	"_SC_MAPPED_FILES",
#endif
#ifndef _SC_MEMLOCK
	"_SC_MEMLOCK",
#endif
#ifndef _SC_MEMLOCK_RANGE
	"_SC_MEMLOCK_RANGE",
#endif
#ifndef _SC_MEMORY_PROTECTION
	"_SC_MEMORY_PROTECTION",
#endif
#ifndef _SC_MESSAGE_PASSING
	"_SC_MESSAGE_PASSING",
#endif
#ifndef _SC_MONOTONIC_CLOCK
	"_SC_MONOTONIC_CLOCK",
#endif
#ifndef _SC_PRIORITIZED_IO
	"_SC_PRIORITIZED_IO",
#endif
#ifndef _SC_PRIORITY_SCHEDULING
	"_SC_PRIORITY_SCHEDULING",
#endif
#ifndef _SC_RAW_SOCKETS
	"_SC_RAW_SOCKETS",
#endif
#ifndef _SC_READER_WRITER_LOCKS
	"_SC_READER_WRITER_LOCKS",
#endif
#ifndef _SC_REALTIME_SIGNALS
	"_SC_REALTIME_SIGNALS",
#endif
#ifndef _SC_REGEXP
	"_SC_REGEXP",
#endif
#ifndef _SC_SAVED_IDS
	"_SC_SAVED_IDS",
#endif
#ifndef _SC_SEMAPHORES
	"_SC_SEMAPHORES",
#endif
#ifndef _SC_SHARED_MEMORY_OBJECTS
	"_SC_SHARED_MEMORY_OBJECTS",
#endif
#ifndef _SC_SHELL
	"_SC_SHELL",
#endif
#ifndef _SC_SPAWN
	"_SC_SPAWN",
#endif
#ifndef _SC_SPIN_LOCKS
	"_SC_SPIN_LOCKS",
#endif
#ifndef _SC_SPORADIC_SERVER
	"_SC_SPORADIC_SERVER",
#endif
#ifndef _SC_SS_REPL_MAX
	"_SC_SS_REPL_MAX",
#endif
#ifndef _SC_SYNCHRONIZED_IO
	"_SC_SYNCHRONIZED_IO",
#endif
#ifndef _SC_THREAD_ATTR_STACKADDR
	"_SC_THREAD_ATTR_STACKADDR",
#endif
#ifndef _SC_THREAD_ATTR_STACKSIZE
	"_SC_THREAD_ATTR_STACKSIZE",
#endif
#ifndef _SC_THREAD_CPUTIME
	"_SC_THREAD_CPUTIME",
#endif
#ifndef _SC_THREAD_PRIO_INHERIT
	"_SC_THREAD_PRIO_INHERIT",
#endif
#ifndef _SC_THREAD_PRIO_PROTECT
	"_SC_THREAD_PRIO_PROTECT",
#endif
#ifndef _SC_THREAD_PRIORITY_SCHEDULING
	"_SC_THREAD_PRIORITY_SCHEDULING",
#endif
#ifndef _SC_THREAD_PROCESS_SHARED
	"_SC_THREAD_PROCESS_SHARED",
#endif
#ifndef _SC_THREAD_ROBUST_PRIO_INHERIT
	"_SC_THREAD_ROBUST_PRIO_INHERIT",
#endif
#ifndef _SC_THREAD_ROBUST_PRIO_PROTECT
	"_SC_THREAD_ROBUST_PRIO_PROTECT",
#endif
#ifndef _SC_THREAD_SAFE_FUNCTIONS
	"_SC_THREAD_SAFE_FUNCTIONS",
#endif
#ifndef _SC_THREAD_SPORADIC_SERVER
	"_SC_THREAD_SPORADIC_SERVER",
#endif
#ifndef _SC_THREADS
	"_SC_THREADS",
#endif
#ifndef _SC_TIMEOUTS
	"_SC_TIMEOUTS",
#endif
#ifndef _SC_TIMERS
	"_SC_TIMERS",
#endif
#ifndef _SC_TYPED_MEMORY_OBJECTS
	"_SC_TYPED_MEMORY_OBJECTS",
#endif
#ifndef _SC_VERSION
	"_SC_VERSION",
#endif
#ifndef _SC_V8_ILP32_OFF32
	"_SC_V8_ILP32_OFF32",
#endif
#ifndef _SC_V8_ILP32_OFFBIG
	"_SC_V8_ILP32_OFFBIG",
#endif
#ifndef _SC_V8_LP64_OFF64
	"_SC_V8_LP64_OFF64",
#endif
#ifndef _SC_V8_LPBIG_OFFBIG
	"_SC_V8_LPBIG_OFFBIG",
#endif
#ifndef _SC_V7_ILP32_OFF32
	"_SC_V7_ILP32_OFF32",
#endif
#ifndef _SC_V7_ILP32_OFFBIG
	"_SC_V7_ILP32_OFFBIG",
#endif
#ifndef _SC_V7_LP64_OFF64
	"_SC_V7_LP64_OFF64",
#endif
#ifndef _SC_V7_LPBIG_OFFBIG
	"_SC_V7_LPBIG_OFFBIG",
#endif
#ifndef _SC_2_C_BIND
	"_SC_2_C_BIND",
#endif
#ifndef _SC_2_C_DEV
	"_SC_2_C_DEV",
#endif
#ifndef _SC_2_CHAR_TERM
	"_SC_2_CHAR_TERM",
#endif
#ifndef _SC_2_FORT_RUN
	"_SC_2_FORT_RUN",
#endif
#ifndef _SC_2_LOCALEDEF
	"_SC_2_LOCALEDEF",
#endif
#ifndef _SC_2_SW_DEV
	"_SC_2_SW_DEV",
#endif
#ifndef _SC_2_UPE
	"_SC_2_UPE",
#endif
#ifndef _SC_2_VERSION
	"_SC_2_VERSION",
#endif
#ifndef _SC_XOPEN_CRYPT
	"_SC_XOPEN_CRYPT",
#endif
#ifndef _SC_XOPEN_ENH_I18N
	"_SC_XOPEN_ENH_I18N",
#endif
#ifndef _SC_XOPEN_REALTIME
	"_SC_XOPEN_REALTIME",
#endif
#ifndef _SC_XOPEN_REALTIME_THREADS
	"_SC_XOPEN_REALTIME_THREADS",
#endif
#ifndef _SC_XOPEN_SHM
	"_SC_XOPEN_SHM",
#endif
#ifndef _SC_XOPEN_UNIX
	"_SC_XOPEN_UNIX",
#endif
#ifndef _SC_XOPEN_UUCP
	"_SC_XOPEN_UUCP",
#endif
#ifndef _SC_XOPEN_VERSION
	"_SC_XOPEN_VERSION",
#endif
	NULL
};

/* 1 if the option a variable depends on ("XSI", "MSG", "SS|TSP") is supported. */
static int supported(const char *dep)
{
	if (dep == NULL)
		return 1;
	if (strcmp(dep, "XSI") == 0)
		return sysconf(_SC_XOPEN_UNIX) > 0;
	if (strcmp(dep, "MSG") == 0)
		return sysconf(_SC_MESSAGE_PASSING) > 0;
	if (strcmp(dep, "SS|TSP") == 0)
		return sysconf(_SC_SPORADIC_SERVER) > 0 || sysconf(_SC_THREAD_SPORADIC_SERVER) > 0;
	return 0;
}

/* Notes the names <unistd.h> lacks (a header matter, XBD <unistd.h>). */
static void note_missing(void)
{
	size_t i;

	for (i = 0; missing[i] != NULL; i++)
		pc_note("<unistd.h> does not define %s; not checked here", missing[i]);
}

/* Compile-time values the headers give for names of the table. */
struct hdr {
	int sc;
	const char *macro;
	long value;
};

static const struct hdr limit_hdr[] = {
#if defined(_SC_AIO_LISTIO_MAX) && defined(AIO_LISTIO_MAX)
	{ _SC_AIO_LISTIO_MAX, "AIO_LISTIO_MAX", (long)(AIO_LISTIO_MAX) },
#endif
#if defined(_SC_AIO_MAX) && defined(AIO_MAX)
	{ _SC_AIO_MAX, "AIO_MAX", (long)(AIO_MAX) },
#endif
#if defined(_SC_AIO_PRIO_DELTA_MAX) && defined(AIO_PRIO_DELTA_MAX)
	{ _SC_AIO_PRIO_DELTA_MAX, "AIO_PRIO_DELTA_MAX", (long)(AIO_PRIO_DELTA_MAX) },
#endif
#if defined(_SC_ARG_MAX) && defined(ARG_MAX)
	{ _SC_ARG_MAX, "ARG_MAX", (long)(ARG_MAX) },
#endif
#if defined(_SC_ATEXIT_MAX) && defined(ATEXIT_MAX)
	{ _SC_ATEXIT_MAX, "ATEXIT_MAX", (long)(ATEXIT_MAX) },
#endif
#if defined(_SC_BC_BASE_MAX) && defined(BC_BASE_MAX)
	{ _SC_BC_BASE_MAX, "BC_BASE_MAX", (long)(BC_BASE_MAX) },
#endif
#if defined(_SC_BC_DIM_MAX) && defined(BC_DIM_MAX)
	{ _SC_BC_DIM_MAX, "BC_DIM_MAX", (long)(BC_DIM_MAX) },
#endif
#if defined(_SC_BC_SCALE_MAX) && defined(BC_SCALE_MAX)
	{ _SC_BC_SCALE_MAX, "BC_SCALE_MAX", (long)(BC_SCALE_MAX) },
#endif
#if defined(_SC_BC_STRING_MAX) && defined(BC_STRING_MAX)
	{ _SC_BC_STRING_MAX, "BC_STRING_MAX", (long)(BC_STRING_MAX) },
#endif
#if defined(_SC_CHILD_MAX) && defined(CHILD_MAX)
	{ _SC_CHILD_MAX, "CHILD_MAX", (long)(CHILD_MAX) },
#endif
#if defined(_SC_COLL_WEIGHTS_MAX) && defined(COLL_WEIGHTS_MAX)
	{ _SC_COLL_WEIGHTS_MAX, "COLL_WEIGHTS_MAX", (long)(COLL_WEIGHTS_MAX) },
#endif
#if defined(_SC_DELAYTIMER_MAX) && defined(DELAYTIMER_MAX)
	{ _SC_DELAYTIMER_MAX, "DELAYTIMER_MAX", (long)(DELAYTIMER_MAX) },
#endif
#if defined(_SC_EXPR_NEST_MAX) && defined(EXPR_NEST_MAX)
	{ _SC_EXPR_NEST_MAX, "EXPR_NEST_MAX", (long)(EXPR_NEST_MAX) },
#endif
#if defined(_SC_HOST_NAME_MAX) && defined(HOST_NAME_MAX)
	{ _SC_HOST_NAME_MAX, "HOST_NAME_MAX", (long)(HOST_NAME_MAX) },
#endif
#if defined(_SC_IOV_MAX) && defined(IOV_MAX)
	{ _SC_IOV_MAX, "IOV_MAX", (long)(IOV_MAX) },
#endif
#if defined(_SC_LINE_MAX) && defined(LINE_MAX)
	{ _SC_LINE_MAX, "LINE_MAX", (long)(LINE_MAX) },
#endif
#if defined(_SC_LOGIN_NAME_MAX) && defined(LOGIN_NAME_MAX)
	{ _SC_LOGIN_NAME_MAX, "LOGIN_NAME_MAX", (long)(LOGIN_NAME_MAX) },
#endif
#if defined(_SC_NGROUPS_MAX) && defined(NGROUPS_MAX)
	{ _SC_NGROUPS_MAX, "NGROUPS_MAX", (long)(NGROUPS_MAX) },
#endif
#if defined(_SC_MQ_OPEN_MAX) && defined(MQ_OPEN_MAX)
	{ _SC_MQ_OPEN_MAX, "MQ_OPEN_MAX", (long)(MQ_OPEN_MAX) },
#endif
#if defined(_SC_MQ_PRIO_MAX) && defined(MQ_PRIO_MAX)
	{ _SC_MQ_PRIO_MAX, "MQ_PRIO_MAX", (long)(MQ_PRIO_MAX) },
#endif
#if defined(_SC_OPEN_MAX) && defined(OPEN_MAX)
	{ _SC_OPEN_MAX, "OPEN_MAX", (long)(OPEN_MAX) },
#endif
#if defined(_SC_PAGE_SIZE) && defined(PAGE_SIZE)
	{ _SC_PAGE_SIZE, "PAGE_SIZE", (long)(PAGE_SIZE) },
#endif
#if defined(_SC_PAGESIZE) && defined(PAGESIZE)
	{ _SC_PAGESIZE, "PAGESIZE", (long)(PAGESIZE) },
#endif
#if defined(_SC_THREAD_DESTRUCTOR_ITERATIONS) && defined(PTHREAD_DESTRUCTOR_ITERATIONS)
	{ _SC_THREAD_DESTRUCTOR_ITERATIONS, "PTHREAD_DESTRUCTOR_ITERATIONS", (long)(PTHREAD_DESTRUCTOR_ITERATIONS) },
#endif
#if defined(_SC_THREAD_KEYS_MAX) && defined(PTHREAD_KEYS_MAX)
	{ _SC_THREAD_KEYS_MAX, "PTHREAD_KEYS_MAX", (long)(PTHREAD_KEYS_MAX) },
#endif
#if defined(_SC_THREAD_STACK_MIN) && defined(PTHREAD_STACK_MIN)
	{ _SC_THREAD_STACK_MIN, "PTHREAD_STACK_MIN", (long)(PTHREAD_STACK_MIN) },
#endif
#if defined(_SC_THREAD_THREADS_MAX) && defined(PTHREAD_THREADS_MAX)
	{ _SC_THREAD_THREADS_MAX, "PTHREAD_THREADS_MAX", (long)(PTHREAD_THREADS_MAX) },
#endif
#if defined(_SC_RE_DUP_MAX) && defined(RE_DUP_MAX)
	{ _SC_RE_DUP_MAX, "RE_DUP_MAX", (long)(RE_DUP_MAX) },
#endif
#if defined(_SC_RTSIG_MAX) && defined(RTSIG_MAX)
	{ _SC_RTSIG_MAX, "RTSIG_MAX", (long)(RTSIG_MAX) },
#endif
#if defined(_SC_SEM_NSEMS_MAX) && defined(SEM_NSEMS_MAX)
	{ _SC_SEM_NSEMS_MAX, "SEM_NSEMS_MAX", (long)(SEM_NSEMS_MAX) },
#endif
#if defined(_SC_SEM_VALUE_MAX) && defined(SEM_VALUE_MAX)
	{ _SC_SEM_VALUE_MAX, "SEM_VALUE_MAX", (long)(SEM_VALUE_MAX) },
#endif
#if defined(_SC_SIGQUEUE_MAX) && defined(SIGQUEUE_MAX)
	{ _SC_SIGQUEUE_MAX, "SIGQUEUE_MAX", (long)(SIGQUEUE_MAX) },
#endif
#if defined(_SC_STREAM_MAX) && defined(STREAM_MAX)
	{ _SC_STREAM_MAX, "STREAM_MAX", (long)(STREAM_MAX) },
#endif
#if defined(_SC_SYMLOOP_MAX) && defined(SYMLOOP_MAX)
	{ _SC_SYMLOOP_MAX, "SYMLOOP_MAX", (long)(SYMLOOP_MAX) },
#endif
#if defined(_SC_TIMER_MAX) && defined(TIMER_MAX)
	{ _SC_TIMER_MAX, "TIMER_MAX", (long)(TIMER_MAX) },
#endif
#if defined(_SC_TTY_NAME_MAX) && defined(TTY_NAME_MAX)
	{ _SC_TTY_NAME_MAX, "TTY_NAME_MAX", (long)(TTY_NAME_MAX) },
#endif
#if defined(_SC_TZNAME_MAX) && defined(TZNAME_MAX)
	{ _SC_TZNAME_MAX, "TZNAME_MAX", (long)(TZNAME_MAX) },
#endif
	{ 0, NULL, 0 }
};

#define SENTINEL 12345

static const struct var *find(int sc)
{
	size_t i;

	for (i = 0; i < sizeof vars / sizeof vars[0]; i++)
		if (vars[i].sc == sc)
			return &vars[i];
	return NULL;
}

int main(void)
{
	const struct var *v;
	int skip_open_max = 0;
	size_t i;
	long r;
	int e;

	pc_begin("XSH.sysconf.003");
	note_missing();
#ifdef OPEN_MAX
	{
		/* {OPEN_MAX} may follow a lower inherited RLIMIT_NOFILE soft limit (Q-42) */
		struct rlimit rl;

		pc_check(getrlimit(RLIMIT_NOFILE, &rl) == 0, "getrlimit failed: %s", pc_errname(errno));
		if (rl.rlim_cur != RLIM_INFINITY && rl.rlim_cur < (rlim_t)OPEN_MAX) {
			if (rl.rlim_max == RLIM_INFINITY || rl.rlim_max >= (rlim_t)OPEN_MAX) {
				rl.rlim_cur = OPEN_MAX;
				pc_check(setrlimit(RLIMIT_NOFILE, &rl) == 0, "setrlimit failed: %s", pc_errname(errno));
			} else {
				pc_note("RLIMIT_NOFILE hard limit below OPEN_MAX: {OPEN_MAX} not checked (Q-42)");
				skip_open_max = 1;
			}
		}
	}
#endif
	pc_test();
	for (i = 0; limit_hdr[i].macro != NULL; i++) {
		v = find(limit_hdr[i].sc);
		if (v == NULL || !supported(v->dep))
			continue;
		if (skip_open_max && strcmp(v->name, "_SC_OPEN_MAX") == 0)
			continue;
		errno = SENTINEL;
		r = sysconf(limit_hdr[i].sc);
		e = errno;
		if (r == -1 && v->kind == 'L' && e == SENTINEL)
			continue;	/* no limit: not more restrictive */
		if (strcmp(limit_hdr[i].macro, "PTHREAD_STACK_MIN") == 0) {
			/* a minimum size: a larger value is more restrictive */
			pc_check(r >= 0 && r <= limit_hdr[i].value,
				 "sysconf(%s) returned %ld; <limits.h> has %s %ld, and a larger minimum is more restrictive",
				 v->name, r, limit_hdr[i].macro, limit_hdr[i].value);
			continue;
		}
		pc_check(r != -1, "sysconf(%s) returned -1 with errno %s", v->name, pc_errname(e));
		pc_check(r >= limit_hdr[i].value, "sysconf(%s) returned %ld, below <limits.h>'s %s %ld",
			 v->name, r, limit_hdr[i].macro, limit_hdr[i].value);
	}
	pc_pass();
}
