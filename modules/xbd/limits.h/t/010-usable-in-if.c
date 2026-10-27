/*
 * Every limits.h name that is defined can be used in #if.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.limits.h.010
 */
#include <limits.h>
#include "pc.h"
/* Undefined names count as 0 here; a macro whose expansion is not valid in #if stops the build. */
#if 1 \
	&& ((AIO_LISTIO_MAX) == (AIO_LISTIO_MAX)) \
	&& ((AIO_MAX) == (AIO_MAX)) \
	&& ((AIO_PRIO_DELTA_MAX) == (AIO_PRIO_DELTA_MAX)) \
	&& ((ARG_MAX) == (ARG_MAX)) \
	&& ((ATEXIT_MAX) == (ATEXIT_MAX)) \
	&& ((CHILD_MAX) == (CHILD_MAX)) \
	&& ((DELAYTIMER_MAX) == (DELAYTIMER_MAX)) \
	&& ((HOST_NAME_MAX) == (HOST_NAME_MAX)) \
	&& ((IOV_MAX) == (IOV_MAX)) \
	&& ((LOGIN_NAME_MAX) == (LOGIN_NAME_MAX)) \
	&& ((MQ_OPEN_MAX) == (MQ_OPEN_MAX)) \
	&& ((MQ_PRIO_MAX) == (MQ_PRIO_MAX)) \
	&& ((OPEN_MAX) == (OPEN_MAX)) \
	&& ((PAGESIZE) == (PAGESIZE)) \
	&& ((PAGE_SIZE) == (PAGE_SIZE)) \
	&& ((PTHREAD_DESTRUCTOR_ITERATIONS) == (PTHREAD_DESTRUCTOR_ITERATIONS)) \
	&& ((PTHREAD_KEYS_MAX) == (PTHREAD_KEYS_MAX)) \
	&& ((PTHREAD_STACK_MIN) == (PTHREAD_STACK_MIN)) \
	&& ((PTHREAD_THREADS_MAX) == (PTHREAD_THREADS_MAX)) \
	&& ((RTSIG_MAX) == (RTSIG_MAX)) \
	&& ((SEM_NSEMS_MAX) == (SEM_NSEMS_MAX)) \
	&& ((SEM_VALUE_MAX) == (SEM_VALUE_MAX)) \
	&& ((SIGQUEUE_MAX) == (SIGQUEUE_MAX)) \
	&& ((SS_REPL_MAX) == (SS_REPL_MAX)) \
	&& ((STREAM_MAX) == (STREAM_MAX)) \
	&& ((SYMLOOP_MAX) == (SYMLOOP_MAX)) \
	&& ((TIMER_MAX) == (TIMER_MAX)) \
	&& ((TTY_NAME_MAX) == (TTY_NAME_MAX)) \
	&& ((TZNAME_MAX) == (TZNAME_MAX)) \
	&& ((FILESIZEBITS) == (FILESIZEBITS)) \
	&& ((LINK_MAX) == (LINK_MAX)) \
	&& ((MAX_CANON) == (MAX_CANON)) \
	&& ((MAX_INPUT) == (MAX_INPUT)) \
	&& ((NAME_MAX) == (NAME_MAX)) \
	&& ((PATH_MAX) == (PATH_MAX)) \
	&& ((PIPE_BUF) == (PIPE_BUF)) \
	&& ((POSIX_ALLOC_SIZE_MIN) == (POSIX_ALLOC_SIZE_MIN)) \
	&& ((POSIX_REC_INCR_XFER_SIZE) == (POSIX_REC_INCR_XFER_SIZE)) \
	&& ((POSIX_REC_MAX_XFER_SIZE) == (POSIX_REC_MAX_XFER_SIZE)) \
	&& ((POSIX_REC_MIN_XFER_SIZE) == (POSIX_REC_MIN_XFER_SIZE)) \
	&& ((POSIX_REC_XFER_ALIGN) == (POSIX_REC_XFER_ALIGN)) \
	&& ((SYMLINK_MAX) == (SYMLINK_MAX)) \
	&& ((TEXTDOMAIN_MAX) == (TEXTDOMAIN_MAX)) \
	&& ((BC_BASE_MAX) == (BC_BASE_MAX)) \
	&& ((BC_DIM_MAX) == (BC_DIM_MAX)) \
	&& ((BC_SCALE_MAX) == (BC_SCALE_MAX)) \
	&& ((BC_STRING_MAX) == (BC_STRING_MAX)) \
	&& ((CHARCLASS_NAME_MAX) == (CHARCLASS_NAME_MAX)) \
	&& ((COLL_WEIGHTS_MAX) == (COLL_WEIGHTS_MAX)) \
	&& ((EXPR_NEST_MAX) == (EXPR_NEST_MAX)) \
	&& ((LINE_MAX) == (LINE_MAX)) \
	&& ((NGROUPS_MAX) == (NGROUPS_MAX)) \
	&& ((RE_DUP_MAX) == (RE_DUP_MAX)) \
	&& ((_POSIX_CLOCKRES_MIN) == (_POSIX_CLOCKRES_MIN)) \
	&& ((_POSIX_AIO_LISTIO_MAX) == (_POSIX_AIO_LISTIO_MAX)) \
	&& ((_POSIX_AIO_MAX) == (_POSIX_AIO_MAX)) \
	&& ((_POSIX_ARG_MAX) == (_POSIX_ARG_MAX)) \
	&& ((_POSIX_CHILD_MAX) == (_POSIX_CHILD_MAX)) \
	&& ((_POSIX_DELAYTIMER_MAX) == (_POSIX_DELAYTIMER_MAX)) \
	&& ((_POSIX_HOST_NAME_MAX) == (_POSIX_HOST_NAME_MAX)) \
	&& ((_POSIX_LINK_MAX) == (_POSIX_LINK_MAX)) \
	&& ((_POSIX_LOGIN_NAME_MAX) == (_POSIX_LOGIN_NAME_MAX)) \
	&& ((_POSIX_MAX_CANON) == (_POSIX_MAX_CANON)) \
	&& ((_POSIX_MAX_INPUT) == (_POSIX_MAX_INPUT)) \
	&& ((_POSIX_MQ_OPEN_MAX) == (_POSIX_MQ_OPEN_MAX)) \
	&& ((_POSIX_MQ_PRIO_MAX) == (_POSIX_MQ_PRIO_MAX)) \
	&& ((_POSIX_NAME_MAX) == (_POSIX_NAME_MAX)) \
	&& ((_POSIX_NGROUPS_MAX) == (_POSIX_NGROUPS_MAX)) \
	&& ((_POSIX_OPEN_MAX) == (_POSIX_OPEN_MAX)) \
	&& ((_POSIX_PATH_MAX) == (_POSIX_PATH_MAX)) \
	&& ((_POSIX_PIPE_BUF) == (_POSIX_PIPE_BUF)) \
	&& ((_POSIX_RE_DUP_MAX) == (_POSIX_RE_DUP_MAX)) \
	&& ((_POSIX_RTSIG_MAX) == (_POSIX_RTSIG_MAX)) \
	&& ((_POSIX_SEM_NSEMS_MAX) == (_POSIX_SEM_NSEMS_MAX)) \
	&& ((_POSIX_SEM_VALUE_MAX) == (_POSIX_SEM_VALUE_MAX)) \
	&& ((_POSIX_SIGQUEUE_MAX) == (_POSIX_SIGQUEUE_MAX)) \
	&& ((_POSIX_SSIZE_MAX) == (_POSIX_SSIZE_MAX)) \
	&& ((_POSIX_SS_REPL_MAX) == (_POSIX_SS_REPL_MAX)) \
	&& ((_POSIX_STREAM_MAX) == (_POSIX_STREAM_MAX)) \
	&& ((_POSIX_SYMLINK_MAX) == (_POSIX_SYMLINK_MAX)) \
	&& ((_POSIX_SYMLOOP_MAX) == (_POSIX_SYMLOOP_MAX)) \
	&& ((_POSIX_THREAD_DESTRUCTOR_ITERATIONS) == (_POSIX_THREAD_DESTRUCTOR_ITERATIONS)) \
	&& ((_POSIX_THREAD_KEYS_MAX) == (_POSIX_THREAD_KEYS_MAX)) \
	&& ((_POSIX_THREAD_THREADS_MAX) == (_POSIX_THREAD_THREADS_MAX)) \
	&& ((_POSIX_TIMER_MAX) == (_POSIX_TIMER_MAX)) \
	&& ((_POSIX_TTY_NAME_MAX) == (_POSIX_TTY_NAME_MAX)) \
	&& ((_POSIX_TZNAME_MAX) == (_POSIX_TZNAME_MAX)) \
	&& ((_POSIX2_BC_BASE_MAX) == (_POSIX2_BC_BASE_MAX)) \
	&& ((_POSIX2_BC_DIM_MAX) == (_POSIX2_BC_DIM_MAX)) \
	&& ((_POSIX2_BC_SCALE_MAX) == (_POSIX2_BC_SCALE_MAX)) \
	&& ((_POSIX2_BC_STRING_MAX) == (_POSIX2_BC_STRING_MAX)) \
	&& ((_POSIX2_CHARCLASS_NAME_MAX) == (_POSIX2_CHARCLASS_NAME_MAX)) \
	&& ((_POSIX2_COLL_WEIGHTS_MAX) == (_POSIX2_COLL_WEIGHTS_MAX)) \
	&& ((_POSIX2_EXPR_NEST_MAX) == (_POSIX2_EXPR_NEST_MAX)) \
	&& ((_POSIX2_LINE_MAX) == (_POSIX2_LINE_MAX)) \
	&& ((_POSIX2_RE_DUP_MAX) == (_POSIX2_RE_DUP_MAX)) \
	&& ((_XOPEN_IOV_MAX) == (_XOPEN_IOV_MAX)) \
	&& ((_XOPEN_NAME_MAX) == (_XOPEN_NAME_MAX)) \
	&& ((_XOPEN_PATH_MAX) == (_XOPEN_PATH_MAX)) \
	&& ((CHAR_BIT) == (CHAR_BIT)) \
	&& ((CHAR_MAX) == (CHAR_MAX)) \
	&& ((CHAR_MIN) == (CHAR_MIN)) \
	&& ((INT_MAX) == (INT_MAX)) \
	&& ((INT_MIN) == (INT_MIN)) \
	&& ((LLONG_MAX) == (LLONG_MAX)) \
	&& ((LLONG_MIN) == (LLONG_MIN)) \
	&& ((LONG_BIT) == (LONG_BIT)) \
	&& ((LONG_MAX) == (LONG_MAX)) \
	&& ((LONG_MIN) == (LONG_MIN)) \
	&& ((MB_LEN_MAX) == (MB_LEN_MAX)) \
	&& ((SCHAR_MAX) == (SCHAR_MAX)) \
	&& ((SCHAR_MIN) == (SCHAR_MIN)) \
	&& ((SHRT_MAX) == (SHRT_MAX)) \
	&& ((SHRT_MIN) == (SHRT_MIN)) \
	&& ((SSIZE_MAX) == (SSIZE_MAX)) \
	&& ((UCHAR_MAX) == (UCHAR_MAX)) \
	&& ((UINT_MAX) == (UINT_MAX)) \
	&& ((ULLONG_MAX) == (ULLONG_MAX)) \
	&& ((ULONG_MAX) == (ULONG_MAX)) \
	&& ((USHRT_MAX) == (USHRT_MAX)) \
	&& ((WORD_BIT) == (WORD_BIT)) \
	&& ((GETENTROPY_MAX) == (GETENTROPY_MAX)) \
	&& ((NL_ARGMAX) == (NL_ARGMAX)) \
	&& ((NL_LANGMAX) == (NL_LANGMAX)) \
	&& ((NL_MSGMAX) == (NL_MSGMAX)) \
	&& ((NL_SETMAX) == (NL_SETMAX)) \
	&& ((NL_TEXTMAX) == (NL_TEXTMAX)) \
	&& ((NSIG_MAX) == (NSIG_MAX)) \
	&& ((NZERO) == (NZERO)) \
	
#endif

int main(void)
{
	pc_begin("XBD.limits.h.010");
	pc_test();
	pc_pass();
}
