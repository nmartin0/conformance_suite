/*
 * Minimum acceptable values of the limits that may be omitted.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.limits.h.011
 */
#include <limits.h>
#include "pc.h"
#if defined(AIO_LISTIO_MAX) && (AIO_LISTIO_MAX) < (_POSIX_AIO_LISTIO_MAX)
#error "AIO_LISTIO_MAX is below its minimum acceptable value"
#endif
#if defined(AIO_MAX) && (AIO_MAX) < (_POSIX_AIO_MAX)
#error "AIO_MAX is below its minimum acceptable value"
#endif
#if defined(AIO_PRIO_DELTA_MAX) && (AIO_PRIO_DELTA_MAX) < (0)
#error "AIO_PRIO_DELTA_MAX is below its minimum acceptable value"
#endif
#if defined(ARG_MAX) && (ARG_MAX) < (_POSIX_ARG_MAX)
#error "ARG_MAX is below its minimum acceptable value"
#endif
#if defined(ATEXIT_MAX) && (ATEXIT_MAX) < (32)
#error "ATEXIT_MAX is below its minimum acceptable value"
#endif
#if defined(CHILD_MAX) && (CHILD_MAX) < (_POSIX_CHILD_MAX)
#error "CHILD_MAX is below its minimum acceptable value"
#endif
#if defined(DELAYTIMER_MAX) && (DELAYTIMER_MAX) < (_POSIX_DELAYTIMER_MAX)
#error "DELAYTIMER_MAX is below its minimum acceptable value"
#endif
#if defined(HOST_NAME_MAX) && (HOST_NAME_MAX) < (_POSIX_HOST_NAME_MAX)
#error "HOST_NAME_MAX is below its minimum acceptable value"
#endif
#if defined(LOGIN_NAME_MAX) && (LOGIN_NAME_MAX) < (_POSIX_LOGIN_NAME_MAX)
#error "LOGIN_NAME_MAX is below its minimum acceptable value"
#endif
#if defined(MQ_OPEN_MAX) && (MQ_OPEN_MAX) < (_POSIX_MQ_OPEN_MAX)
#error "MQ_OPEN_MAX is below its minimum acceptable value"
#endif
#if defined(MQ_PRIO_MAX) && (MQ_PRIO_MAX) < (_POSIX_MQ_PRIO_MAX)
#error "MQ_PRIO_MAX is below its minimum acceptable value"
#endif
#if defined(OPEN_MAX) && (OPEN_MAX) < (_POSIX_OPEN_MAX)
#error "OPEN_MAX is below its minimum acceptable value"
#endif
#if defined(PAGESIZE) && (PAGESIZE) < (1)
#error "PAGESIZE is below its minimum acceptable value"
#endif
#if defined(PTHREAD_DESTRUCTOR_ITERATIONS) && (PTHREAD_DESTRUCTOR_ITERATIONS) < (_POSIX_THREAD_DESTRUCTOR_ITERATIONS)
#error "PTHREAD_DESTRUCTOR_ITERATIONS is below its minimum acceptable value"
#endif
#if defined(PTHREAD_KEYS_MAX) && (PTHREAD_KEYS_MAX) < (_POSIX_THREAD_KEYS_MAX)
#error "PTHREAD_KEYS_MAX is below its minimum acceptable value"
#endif
#if defined(PTHREAD_STACK_MIN) && (PTHREAD_STACK_MIN) < (0)
#error "PTHREAD_STACK_MIN is below its minimum acceptable value"
#endif
#if defined(PTHREAD_THREADS_MAX) && (PTHREAD_THREADS_MAX) < (_POSIX_THREAD_THREADS_MAX)
#error "PTHREAD_THREADS_MAX is below its minimum acceptable value"
#endif
#if defined(RTSIG_MAX) && (RTSIG_MAX) < (_POSIX_RTSIG_MAX)
#error "RTSIG_MAX is below its minimum acceptable value"
#endif
#if defined(SEM_NSEMS_MAX) && (SEM_NSEMS_MAX) < (_POSIX_SEM_NSEMS_MAX)
#error "SEM_NSEMS_MAX is below its minimum acceptable value"
#endif
#if defined(SEM_VALUE_MAX) && (SEM_VALUE_MAX) < (_POSIX_SEM_VALUE_MAX)
#error "SEM_VALUE_MAX is below its minimum acceptable value"
#endif
#if defined(SIGQUEUE_MAX) && (SIGQUEUE_MAX) < (_POSIX_SIGQUEUE_MAX)
#error "SIGQUEUE_MAX is below its minimum acceptable value"
#endif
#if defined(SS_REPL_MAX) && (SS_REPL_MAX) < (_POSIX_SS_REPL_MAX)
#error "SS_REPL_MAX is below its minimum acceptable value"
#endif
#if defined(STREAM_MAX) && (STREAM_MAX) < (_POSIX_STREAM_MAX)
#error "STREAM_MAX is below its minimum acceptable value"
#endif
#if defined(SYMLOOP_MAX) && (SYMLOOP_MAX) < (_POSIX_SYMLOOP_MAX)
#error "SYMLOOP_MAX is below its minimum acceptable value"
#endif
#if defined(TIMER_MAX) && (TIMER_MAX) < (_POSIX_TIMER_MAX)
#error "TIMER_MAX is below its minimum acceptable value"
#endif
#if defined(TTY_NAME_MAX) && (TTY_NAME_MAX) < (_POSIX_TTY_NAME_MAX)
#error "TTY_NAME_MAX is below its minimum acceptable value"
#endif
#if defined(TZNAME_MAX) && (TZNAME_MAX) < (_POSIX_TZNAME_MAX)
#error "TZNAME_MAX is below its minimum acceptable value"
#endif
#if defined(FILESIZEBITS) && (FILESIZEBITS) < (32)
#error "FILESIZEBITS is below its minimum acceptable value"
#endif
#if defined(LINK_MAX) && (LINK_MAX) < (_POSIX_LINK_MAX)
#error "LINK_MAX is below its minimum acceptable value"
#endif
#if defined(MAX_CANON) && (MAX_CANON) < (_POSIX_MAX_CANON)
#error "MAX_CANON is below its minimum acceptable value"
#endif
#if defined(MAX_INPUT) && (MAX_INPUT) < (_POSIX_MAX_INPUT)
#error "MAX_INPUT is below its minimum acceptable value"
#endif
#if defined(NAME_MAX) && (NAME_MAX) < (_POSIX_NAME_MAX)
#error "NAME_MAX is below its minimum acceptable value"
#endif
#if defined(PATH_MAX) && (PATH_MAX) < (_POSIX_PATH_MAX)
#error "PATH_MAX is below its minimum acceptable value"
#endif
#if defined(PIPE_BUF) && (PIPE_BUF) < (_POSIX_PIPE_BUF)
#error "PIPE_BUF is below its minimum acceptable value"
#endif
#if defined(SYMLINK_MAX) && (SYMLINK_MAX) < (_POSIX_SYMLINK_MAX)
#error "SYMLINK_MAX is below its minimum acceptable value"
#endif
#if defined(TEXTDOMAIN_MAX) && (TEXTDOMAIN_MAX) < (_POSIX_NAME_MAX - 3)
#error "TEXTDOMAIN_MAX is below its minimum acceptable value"
#endif
#if defined(BC_BASE_MAX) && (BC_BASE_MAX) < (_POSIX2_BC_BASE_MAX)
#error "BC_BASE_MAX is below its minimum acceptable value"
#endif
#if defined(BC_DIM_MAX) && (BC_DIM_MAX) < (_POSIX2_BC_DIM_MAX)
#error "BC_DIM_MAX is below its minimum acceptable value"
#endif
#if defined(BC_SCALE_MAX) && (BC_SCALE_MAX) < (_POSIX2_BC_SCALE_MAX)
#error "BC_SCALE_MAX is below its minimum acceptable value"
#endif
#if defined(BC_STRING_MAX) && (BC_STRING_MAX) < (_POSIX2_BC_STRING_MAX)
#error "BC_STRING_MAX is below its minimum acceptable value"
#endif
#if defined(CHARCLASS_NAME_MAX) && (CHARCLASS_NAME_MAX) < (_POSIX2_CHARCLASS_NAME_MAX)
#error "CHARCLASS_NAME_MAX is below its minimum acceptable value"
#endif
#if defined(COLL_WEIGHTS_MAX) && (COLL_WEIGHTS_MAX) < (_POSIX2_COLL_WEIGHTS_MAX)
#error "COLL_WEIGHTS_MAX is below its minimum acceptable value"
#endif
#if defined(EXPR_NEST_MAX) && (EXPR_NEST_MAX) < (_POSIX2_EXPR_NEST_MAX)
#error "EXPR_NEST_MAX is below its minimum acceptable value"
#endif
#if defined(LINE_MAX) && (LINE_MAX) < (_POSIX2_LINE_MAX)
#error "LINE_MAX is below its minimum acceptable value"
#endif
#if defined(NGROUPS_MAX) && (NGROUPS_MAX) < (_POSIX_NGROUPS_MAX)
#error "NGROUPS_MAX is below its minimum acceptable value"
#endif
#if defined(RE_DUP_MAX) && (RE_DUP_MAX) < (_POSIX_RE_DUP_MAX)
#error "RE_DUP_MAX is below its minimum acceptable value"
#endif

int main(void)
{
	pc_begin("XBD.limits.h.011");
	pc_test();
	pc_pass();
}
