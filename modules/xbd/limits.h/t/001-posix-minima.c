/*
 * The fixed _POSIX_* minimum values.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.limits.h.001
 */
#include <limits.h>
#include "pc.h"
#if !defined(_POSIX_AIO_LISTIO_MAX) || (_POSIX_AIO_LISTIO_MAX) != 2
#error "_POSIX_AIO_LISTIO_MAX is missing or not the required value"
#endif
#if !defined(_POSIX_AIO_MAX) || (_POSIX_AIO_MAX) != 1
#error "_POSIX_AIO_MAX is missing or not the required value"
#endif
#if !defined(_POSIX_ARG_MAX) || (_POSIX_ARG_MAX) != 4096
#error "_POSIX_ARG_MAX is missing or not the required value"
#endif
#if !defined(_POSIX_CHILD_MAX) || (_POSIX_CHILD_MAX) != 25
#error "_POSIX_CHILD_MAX is missing or not the required value"
#endif
#if !defined(_POSIX_DELAYTIMER_MAX) || (_POSIX_DELAYTIMER_MAX) != 32
#error "_POSIX_DELAYTIMER_MAX is missing or not the required value"
#endif
#if !defined(_POSIX_HOST_NAME_MAX) || (_POSIX_HOST_NAME_MAX) != 255
#error "_POSIX_HOST_NAME_MAX is missing or not the required value"
#endif
#if !defined(_POSIX_LINK_MAX) || (_POSIX_LINK_MAX) != 8
#error "_POSIX_LINK_MAX is missing or not the required value"
#endif
#if !defined(_POSIX_LOGIN_NAME_MAX) || (_POSIX_LOGIN_NAME_MAX) != 9
#error "_POSIX_LOGIN_NAME_MAX is missing or not the required value"
#endif
#if !defined(_POSIX_MAX_CANON) || (_POSIX_MAX_CANON) != 255
#error "_POSIX_MAX_CANON is missing or not the required value"
#endif
#if !defined(_POSIX_MAX_INPUT) || (_POSIX_MAX_INPUT) != 255
#error "_POSIX_MAX_INPUT is missing or not the required value"
#endif
#if !defined(_POSIX_NAME_MAX) || (_POSIX_NAME_MAX) != 14
#error "_POSIX_NAME_MAX is missing or not the required value"
#endif
#if !defined(_POSIX_NGROUPS_MAX) || (_POSIX_NGROUPS_MAX) != 8
#error "_POSIX_NGROUPS_MAX is missing or not the required value"
#endif
#if !defined(_POSIX_OPEN_MAX) || (_POSIX_OPEN_MAX) != 20
#error "_POSIX_OPEN_MAX is missing or not the required value"
#endif
#if !defined(_POSIX_PATH_MAX) || (_POSIX_PATH_MAX) != 256
#error "_POSIX_PATH_MAX is missing or not the required value"
#endif
#if !defined(_POSIX_PIPE_BUF) || (_POSIX_PIPE_BUF) != 512
#error "_POSIX_PIPE_BUF is missing or not the required value"
#endif
#if !defined(_POSIX_RE_DUP_MAX) || (_POSIX_RE_DUP_MAX) != 255
#error "_POSIX_RE_DUP_MAX is missing or not the required value"
#endif
#if !defined(_POSIX_RTSIG_MAX) || (_POSIX_RTSIG_MAX) != 8
#error "_POSIX_RTSIG_MAX is missing or not the required value"
#endif
#if !defined(_POSIX_SEM_NSEMS_MAX) || (_POSIX_SEM_NSEMS_MAX) != 256
#error "_POSIX_SEM_NSEMS_MAX is missing or not the required value"
#endif
#if !defined(_POSIX_SEM_VALUE_MAX) || (_POSIX_SEM_VALUE_MAX) != 32767
#error "_POSIX_SEM_VALUE_MAX is missing or not the required value"
#endif
#if !defined(_POSIX_SIGQUEUE_MAX) || (_POSIX_SIGQUEUE_MAX) != 32
#error "_POSIX_SIGQUEUE_MAX is missing or not the required value"
#endif
#if !defined(_POSIX_SSIZE_MAX) || (_POSIX_SSIZE_MAX) != 32767
#error "_POSIX_SSIZE_MAX is missing or not the required value"
#endif
#if !defined(_POSIX_STREAM_MAX) || (_POSIX_STREAM_MAX) != 8
#error "_POSIX_STREAM_MAX is missing or not the required value"
#endif
#if !defined(_POSIX_SYMLINK_MAX) || (_POSIX_SYMLINK_MAX) != 255
#error "_POSIX_SYMLINK_MAX is missing or not the required value"
#endif
#if !defined(_POSIX_SYMLOOP_MAX) || (_POSIX_SYMLOOP_MAX) != 8
#error "_POSIX_SYMLOOP_MAX is missing or not the required value"
#endif
#if !defined(_POSIX_THREAD_DESTRUCTOR_ITERATIONS) || (_POSIX_THREAD_DESTRUCTOR_ITERATIONS) != 4
#error "_POSIX_THREAD_DESTRUCTOR_ITERATIONS is missing or not the required value"
#endif
#if !defined(_POSIX_THREAD_KEYS_MAX) || (_POSIX_THREAD_KEYS_MAX) != 128
#error "_POSIX_THREAD_KEYS_MAX is missing or not the required value"
#endif
#if !defined(_POSIX_THREAD_THREADS_MAX) || (_POSIX_THREAD_THREADS_MAX) != 64
#error "_POSIX_THREAD_THREADS_MAX is missing or not the required value"
#endif
#if !defined(_POSIX_TIMER_MAX) || (_POSIX_TIMER_MAX) != 32
#error "_POSIX_TIMER_MAX is missing or not the required value"
#endif
#if !defined(_POSIX_TTY_NAME_MAX) || (_POSIX_TTY_NAME_MAX) != 9
#error "_POSIX_TTY_NAME_MAX is missing or not the required value"
#endif
#if !defined(_POSIX_TZNAME_MAX) || (_POSIX_TZNAME_MAX) != 6
#error "_POSIX_TZNAME_MAX is missing or not the required value"
#endif
#if !defined(_POSIX_CLOCKRES_MIN) || (_POSIX_CLOCKRES_MIN) != 20000000
#error "_POSIX_CLOCKRES_MIN is missing or not the required value"
#endif

int main(void)
{
	pc_begin("XBD.limits.h.001");
	pc_test();
	pc_pass();
}
