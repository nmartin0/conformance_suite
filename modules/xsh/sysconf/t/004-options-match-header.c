/*
 * An option <unistd.h> defines greater than zero is reported as
 * supported by sysconf().
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sysconf.004
 * MUTANT    sysconf.option_off
 * MUTANT    sysconf.job_control_zero
 * VARIANT   sysconf.errno_on_success
 */
#include "pc.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>
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

static const struct hdr option_hdr[] = {
#if defined(_SC_ADVISORY_INFO) && defined(_POSIX_ADVISORY_INFO)
	{ _SC_ADVISORY_INFO, "_POSIX_ADVISORY_INFO", (long)(_POSIX_ADVISORY_INFO) },
#endif
#if defined(_SC_BARRIERS) && defined(_POSIX_BARRIERS)
	{ _SC_BARRIERS, "_POSIX_BARRIERS", (long)(_POSIX_BARRIERS) },
#endif
#if defined(_SC_ASYNCHRONOUS_IO) && defined(_POSIX_ASYNCHRONOUS_IO)
	{ _SC_ASYNCHRONOUS_IO, "_POSIX_ASYNCHRONOUS_IO", (long)(_POSIX_ASYNCHRONOUS_IO) },
#endif
#if defined(_SC_CLOCK_SELECTION) && defined(_POSIX_CLOCK_SELECTION)
	{ _SC_CLOCK_SELECTION, "_POSIX_CLOCK_SELECTION", (long)(_POSIX_CLOCK_SELECTION) },
#endif
#if defined(_SC_CPUTIME) && defined(_POSIX_CPUTIME)
	{ _SC_CPUTIME, "_POSIX_CPUTIME", (long)(_POSIX_CPUTIME) },
#endif
#if defined(_SC_DEVICE_CONTROL) && defined(_POSIX_DEVICE_CONTROL)
	{ _SC_DEVICE_CONTROL, "_POSIX_DEVICE_CONTROL", (long)(_POSIX_DEVICE_CONTROL) },
#endif
#if defined(_SC_FSYNC) && defined(_POSIX_FSYNC)
	{ _SC_FSYNC, "_POSIX_FSYNC", (long)(_POSIX_FSYNC) },
#endif
#if defined(_SC_IPV6) && defined(_POSIX_IPV6)
	{ _SC_IPV6, "_POSIX_IPV6", (long)(_POSIX_IPV6) },
#endif
#if defined(_SC_JOB_CONTROL) && defined(_POSIX_JOB_CONTROL)
	{ _SC_JOB_CONTROL, "_POSIX_JOB_CONTROL", (long)(_POSIX_JOB_CONTROL) },
#endif
#if defined(_SC_MAPPED_FILES) && defined(_POSIX_MAPPED_FILES)
	{ _SC_MAPPED_FILES, "_POSIX_MAPPED_FILES", (long)(_POSIX_MAPPED_FILES) },
#endif
#if defined(_SC_MEMLOCK) && defined(_POSIX_MEMLOCK)
	{ _SC_MEMLOCK, "_POSIX_MEMLOCK", (long)(_POSIX_MEMLOCK) },
#endif
#if defined(_SC_MEMLOCK_RANGE) && defined(_POSIX_MEMLOCK_RANGE)
	{ _SC_MEMLOCK_RANGE, "_POSIX_MEMLOCK_RANGE", (long)(_POSIX_MEMLOCK_RANGE) },
#endif
#if defined(_SC_MEMORY_PROTECTION) && defined(_POSIX_MEMORY_PROTECTION)
	{ _SC_MEMORY_PROTECTION, "_POSIX_MEMORY_PROTECTION", (long)(_POSIX_MEMORY_PROTECTION) },
#endif
#if defined(_SC_MESSAGE_PASSING) && defined(_POSIX_MESSAGE_PASSING)
	{ _SC_MESSAGE_PASSING, "_POSIX_MESSAGE_PASSING", (long)(_POSIX_MESSAGE_PASSING) },
#endif
#if defined(_SC_MONOTONIC_CLOCK) && defined(_POSIX_MONOTONIC_CLOCK)
	{ _SC_MONOTONIC_CLOCK, "_POSIX_MONOTONIC_CLOCK", (long)(_POSIX_MONOTONIC_CLOCK) },
#endif
#if defined(_SC_PRIORITIZED_IO) && defined(_POSIX_PRIORITIZED_IO)
	{ _SC_PRIORITIZED_IO, "_POSIX_PRIORITIZED_IO", (long)(_POSIX_PRIORITIZED_IO) },
#endif
#if defined(_SC_PRIORITY_SCHEDULING) && defined(_POSIX_PRIORITY_SCHEDULING)
	{ _SC_PRIORITY_SCHEDULING, "_POSIX_PRIORITY_SCHEDULING", (long)(_POSIX_PRIORITY_SCHEDULING) },
#endif
#if defined(_SC_RAW_SOCKETS) && defined(_POSIX_RAW_SOCKETS)
	{ _SC_RAW_SOCKETS, "_POSIX_RAW_SOCKETS", (long)(_POSIX_RAW_SOCKETS) },
#endif
#if defined(_SC_READER_WRITER_LOCKS) && defined(_POSIX_READER_WRITER_LOCKS)
	{ _SC_READER_WRITER_LOCKS, "_POSIX_READER_WRITER_LOCKS", (long)(_POSIX_READER_WRITER_LOCKS) },
#endif
#if defined(_SC_REALTIME_SIGNALS) && defined(_POSIX_REALTIME_SIGNALS)
	{ _SC_REALTIME_SIGNALS, "_POSIX_REALTIME_SIGNALS", (long)(_POSIX_REALTIME_SIGNALS) },
#endif
#if defined(_SC_REGEXP) && defined(_POSIX_REGEXP)
	{ _SC_REGEXP, "_POSIX_REGEXP", (long)(_POSIX_REGEXP) },
#endif
#if defined(_SC_SAVED_IDS) && defined(_POSIX_SAVED_IDS)
	{ _SC_SAVED_IDS, "_POSIX_SAVED_IDS", (long)(_POSIX_SAVED_IDS) },
#endif
#if defined(_SC_SEMAPHORES) && defined(_POSIX_SEMAPHORES)
	{ _SC_SEMAPHORES, "_POSIX_SEMAPHORES", (long)(_POSIX_SEMAPHORES) },
#endif
#if defined(_SC_SHARED_MEMORY_OBJECTS) && defined(_POSIX_SHARED_MEMORY_OBJECTS)
	{ _SC_SHARED_MEMORY_OBJECTS, "_POSIX_SHARED_MEMORY_OBJECTS", (long)(_POSIX_SHARED_MEMORY_OBJECTS) },
#endif
#if defined(_SC_SHELL) && defined(_POSIX_SHELL)
	{ _SC_SHELL, "_POSIX_SHELL", (long)(_POSIX_SHELL) },
#endif
#if defined(_SC_SPAWN) && defined(_POSIX_SPAWN)
	{ _SC_SPAWN, "_POSIX_SPAWN", (long)(_POSIX_SPAWN) },
#endif
#if defined(_SC_SPIN_LOCKS) && defined(_POSIX_SPIN_LOCKS)
	{ _SC_SPIN_LOCKS, "_POSIX_SPIN_LOCKS", (long)(_POSIX_SPIN_LOCKS) },
#endif
#if defined(_SC_SPORADIC_SERVER) && defined(_POSIX_SPORADIC_SERVER)
	{ _SC_SPORADIC_SERVER, "_POSIX_SPORADIC_SERVER", (long)(_POSIX_SPORADIC_SERVER) },
#endif
#if defined(_SC_SYNCHRONIZED_IO) && defined(_POSIX_SYNCHRONIZED_IO)
	{ _SC_SYNCHRONIZED_IO, "_POSIX_SYNCHRONIZED_IO", (long)(_POSIX_SYNCHRONIZED_IO) },
#endif
#if defined(_SC_THREAD_ATTR_STACKADDR) && defined(_POSIX_THREAD_ATTR_STACKADDR)
	{ _SC_THREAD_ATTR_STACKADDR, "_POSIX_THREAD_ATTR_STACKADDR", (long)(_POSIX_THREAD_ATTR_STACKADDR) },
#endif
#if defined(_SC_THREAD_ATTR_STACKSIZE) && defined(_POSIX_THREAD_ATTR_STACKSIZE)
	{ _SC_THREAD_ATTR_STACKSIZE, "_POSIX_THREAD_ATTR_STACKSIZE", (long)(_POSIX_THREAD_ATTR_STACKSIZE) },
#endif
#if defined(_SC_THREAD_CPUTIME) && defined(_POSIX_THREAD_CPUTIME)
	{ _SC_THREAD_CPUTIME, "_POSIX_THREAD_CPUTIME", (long)(_POSIX_THREAD_CPUTIME) },
#endif
#if defined(_SC_THREAD_PRIO_INHERIT) && defined(_POSIX_THREAD_PRIO_INHERIT)
	{ _SC_THREAD_PRIO_INHERIT, "_POSIX_THREAD_PRIO_INHERIT", (long)(_POSIX_THREAD_PRIO_INHERIT) },
#endif
#if defined(_SC_THREAD_PRIO_PROTECT) && defined(_POSIX_THREAD_PRIO_PROTECT)
	{ _SC_THREAD_PRIO_PROTECT, "_POSIX_THREAD_PRIO_PROTECT", (long)(_POSIX_THREAD_PRIO_PROTECT) },
#endif
#if defined(_SC_THREAD_PRIORITY_SCHEDULING) && defined(_POSIX_THREAD_PRIORITY_SCHEDULING)
	{ _SC_THREAD_PRIORITY_SCHEDULING, "_POSIX_THREAD_PRIORITY_SCHEDULING", (long)(_POSIX_THREAD_PRIORITY_SCHEDULING) },
#endif
#if defined(_SC_THREAD_PROCESS_SHARED) && defined(_POSIX_THREAD_PROCESS_SHARED)
	{ _SC_THREAD_PROCESS_SHARED, "_POSIX_THREAD_PROCESS_SHARED", (long)(_POSIX_THREAD_PROCESS_SHARED) },
#endif
#if defined(_SC_THREAD_ROBUST_PRIO_INHERIT) && defined(_POSIX_THREAD_ROBUST_PRIO_INHERIT)
	{ _SC_THREAD_ROBUST_PRIO_INHERIT, "_POSIX_THREAD_ROBUST_PRIO_INHERIT", (long)(_POSIX_THREAD_ROBUST_PRIO_INHERIT) },
#endif
#if defined(_SC_THREAD_ROBUST_PRIO_PROTECT) && defined(_POSIX_THREAD_ROBUST_PRIO_PROTECT)
	{ _SC_THREAD_ROBUST_PRIO_PROTECT, "_POSIX_THREAD_ROBUST_PRIO_PROTECT", (long)(_POSIX_THREAD_ROBUST_PRIO_PROTECT) },
#endif
#if defined(_SC_THREAD_SAFE_FUNCTIONS) && defined(_POSIX_THREAD_SAFE_FUNCTIONS)
	{ _SC_THREAD_SAFE_FUNCTIONS, "_POSIX_THREAD_SAFE_FUNCTIONS", (long)(_POSIX_THREAD_SAFE_FUNCTIONS) },
#endif
#if defined(_SC_THREAD_SPORADIC_SERVER) && defined(_POSIX_THREAD_SPORADIC_SERVER)
	{ _SC_THREAD_SPORADIC_SERVER, "_POSIX_THREAD_SPORADIC_SERVER", (long)(_POSIX_THREAD_SPORADIC_SERVER) },
#endif
#if defined(_SC_THREADS) && defined(_POSIX_THREADS)
	{ _SC_THREADS, "_POSIX_THREADS", (long)(_POSIX_THREADS) },
#endif
#if defined(_SC_TIMEOUTS) && defined(_POSIX_TIMEOUTS)
	{ _SC_TIMEOUTS, "_POSIX_TIMEOUTS", (long)(_POSIX_TIMEOUTS) },
#endif
#if defined(_SC_TIMERS) && defined(_POSIX_TIMERS)
	{ _SC_TIMERS, "_POSIX_TIMERS", (long)(_POSIX_TIMERS) },
#endif
#if defined(_SC_TYPED_MEMORY_OBJECTS) && defined(_POSIX_TYPED_MEMORY_OBJECTS)
	{ _SC_TYPED_MEMORY_OBJECTS, "_POSIX_TYPED_MEMORY_OBJECTS", (long)(_POSIX_TYPED_MEMORY_OBJECTS) },
#endif
#if defined(_SC_VERSION) && defined(_POSIX_VERSION)
	{ _SC_VERSION, "_POSIX_VERSION", (long)(_POSIX_VERSION) },
#endif
#if defined(_SC_V8_ILP32_OFF32) && defined(_POSIX_V8_ILP32_OFF32)
	{ _SC_V8_ILP32_OFF32, "_POSIX_V8_ILP32_OFF32", (long)(_POSIX_V8_ILP32_OFF32) },
#endif
#if defined(_SC_V8_ILP32_OFFBIG) && defined(_POSIX_V8_ILP32_OFFBIG)
	{ _SC_V8_ILP32_OFFBIG, "_POSIX_V8_ILP32_OFFBIG", (long)(_POSIX_V8_ILP32_OFFBIG) },
#endif
#if defined(_SC_V8_LP64_OFF64) && defined(_POSIX_V8_LP64_OFF64)
	{ _SC_V8_LP64_OFF64, "_POSIX_V8_LP64_OFF64", (long)(_POSIX_V8_LP64_OFF64) },
#endif
#if defined(_SC_V8_LPBIG_OFFBIG) && defined(_POSIX_V8_LPBIG_OFFBIG)
	{ _SC_V8_LPBIG_OFFBIG, "_POSIX_V8_LPBIG_OFFBIG", (long)(_POSIX_V8_LPBIG_OFFBIG) },
#endif
#if defined(_SC_V7_ILP32_OFF32) && defined(_POSIX_V7_ILP32_OFF32)
	{ _SC_V7_ILP32_OFF32, "_POSIX_V7_ILP32_OFF32", (long)(_POSIX_V7_ILP32_OFF32) },
#endif
#if defined(_SC_V7_ILP32_OFFBIG) && defined(_POSIX_V7_ILP32_OFFBIG)
	{ _SC_V7_ILP32_OFFBIG, "_POSIX_V7_ILP32_OFFBIG", (long)(_POSIX_V7_ILP32_OFFBIG) },
#endif
#if defined(_SC_V7_LP64_OFF64) && defined(_POSIX_V7_LP64_OFF64)
	{ _SC_V7_LP64_OFF64, "_POSIX_V7_LP64_OFF64", (long)(_POSIX_V7_LP64_OFF64) },
#endif
#if defined(_SC_V7_LPBIG_OFFBIG) && defined(_POSIX_V7_LPBIG_OFFBIG)
	{ _SC_V7_LPBIG_OFFBIG, "_POSIX_V7_LPBIG_OFFBIG", (long)(_POSIX_V7_LPBIG_OFFBIG) },
#endif
#if defined(_SC_2_C_BIND) && defined(_POSIX2_C_BIND)
	{ _SC_2_C_BIND, "_POSIX2_C_BIND", (long)(_POSIX2_C_BIND) },
#endif
#if defined(_SC_2_C_DEV) && defined(_POSIX2_C_DEV)
	{ _SC_2_C_DEV, "_POSIX2_C_DEV", (long)(_POSIX2_C_DEV) },
#endif
#if defined(_SC_2_CHAR_TERM) && defined(_POSIX2_CHAR_TERM)
	{ _SC_2_CHAR_TERM, "_POSIX2_CHAR_TERM", (long)(_POSIX2_CHAR_TERM) },
#endif
#if defined(_SC_2_FORT_RUN) && defined(_POSIX2_FORT_RUN)
	{ _SC_2_FORT_RUN, "_POSIX2_FORT_RUN", (long)(_POSIX2_FORT_RUN) },
#endif
#if defined(_SC_2_LOCALEDEF) && defined(_POSIX2_LOCALEDEF)
	{ _SC_2_LOCALEDEF, "_POSIX2_LOCALEDEF", (long)(_POSIX2_LOCALEDEF) },
#endif
#if defined(_SC_2_SW_DEV) && defined(_POSIX2_SW_DEV)
	{ _SC_2_SW_DEV, "_POSIX2_SW_DEV", (long)(_POSIX2_SW_DEV) },
#endif
#if defined(_SC_2_UPE) && defined(_POSIX2_UPE)
	{ _SC_2_UPE, "_POSIX2_UPE", (long)(_POSIX2_UPE) },
#endif
#if defined(_SC_2_VERSION) && defined(_POSIX2_VERSION)
	{ _SC_2_VERSION, "_POSIX2_VERSION", (long)(_POSIX2_VERSION) },
#endif
#if defined(_SC_XOPEN_CRYPT) && defined(_XOPEN_CRYPT)
	{ _SC_XOPEN_CRYPT, "_XOPEN_CRYPT", (long)(_XOPEN_CRYPT) },
#endif
#if defined(_SC_XOPEN_ENH_I18N) && defined(_XOPEN_ENH_I18N)
	{ _SC_XOPEN_ENH_I18N, "_XOPEN_ENH_I18N", (long)(_XOPEN_ENH_I18N) },
#endif
#if defined(_SC_XOPEN_REALTIME) && defined(_XOPEN_REALTIME)
	{ _SC_XOPEN_REALTIME, "_XOPEN_REALTIME", (long)(_XOPEN_REALTIME) },
#endif
#if defined(_SC_XOPEN_REALTIME_THREADS) && defined(_XOPEN_REALTIME_THREADS)
	{ _SC_XOPEN_REALTIME_THREADS, "_XOPEN_REALTIME_THREADS", (long)(_XOPEN_REALTIME_THREADS) },
#endif
#if defined(_SC_XOPEN_SHM) && defined(_XOPEN_SHM)
	{ _SC_XOPEN_SHM, "_XOPEN_SHM", (long)(_XOPEN_SHM) },
#endif
#if defined(_SC_XOPEN_UNIX) && defined(_XOPEN_UNIX)
	{ _SC_XOPEN_UNIX, "_XOPEN_UNIX", (long)(_XOPEN_UNIX) },
#endif
#if defined(_SC_XOPEN_UUCP) && defined(_XOPEN_UUCP)
	{ _SC_XOPEN_UUCP, "_XOPEN_UUCP", (long)(_XOPEN_UUCP) },
#endif
#if defined(_SC_XOPEN_VERSION) && defined(_XOPEN_VERSION)
	{ _SC_XOPEN_VERSION, "_XOPEN_VERSION", (long)(_XOPEN_VERSION) },
#endif
	{ 0, NULL, 0 }
};

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
	size_t i;
	long r;

	pc_begin("XSH.sysconf.004");
	note_missing();
	pc_test();
	for (i = 0; option_hdr[i].macro != NULL; i++) {
		v = find(option_hdr[i].sc);
		if (v == NULL || v->kind == 'V' || option_hdr[i].value <= 0 || !supported(v->dep))
			continue;
		r = sysconf(option_hdr[i].sc);
		pc_check(r > 0, "<unistd.h> defines %s as %ld (always supported) but sysconf(%s) returns %ld",
			 option_hdr[i].macro, option_hdr[i].value, v->name, r);
	}
	pc_pass();
}
