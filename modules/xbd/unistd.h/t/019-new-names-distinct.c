/*
 * The 2024 additions to _SC_, _PC_ and _CS_ are distinct.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.unistd.h.019
 */
#include <string.h>
#include <unistd.h>
#include "pc.h"

struct c { const char *name; long val; };
static const struct c new_SC[] = {
	{ "_SC_NPROCESSORS_CONF", (long)_SC_NPROCESSORS_CONF },
	{ "_SC_NPROCESSORS_ONLN", (long)_SC_NPROCESSORS_ONLN },
	{ "_SC_DEVICE_CONTROL", (long)_SC_DEVICE_CONTROL },
	{ "_SC_NSIG", (long)_SC_NSIG },
	{ "_SC_V8_ILP32_OFF32", (long)_SC_V8_ILP32_OFF32 },
	{ "_SC_V8_ILP32_OFFBIG", (long)_SC_V8_ILP32_OFFBIG },
	{ "_SC_V8_LP64_OFF64", (long)_SC_V8_LP64_OFF64 },
	{ "_SC_V8_LPBIG_OFFBIG", (long)_SC_V8_LPBIG_OFFBIG },
};
static const struct c old_SC[] = {
	{ "_SC_2_C_BIND", (long)_SC_2_C_BIND },
	{ "_SC_2_C_DEV", (long)_SC_2_C_DEV },
	{ "_SC_2_CHAR_TERM", (long)_SC_2_CHAR_TERM },
	{ "_SC_2_FORT_RUN", (long)_SC_2_FORT_RUN },
	{ "_SC_2_LOCALEDEF", (long)_SC_2_LOCALEDEF },
	{ "_SC_2_SW_DEV", (long)_SC_2_SW_DEV },
	{ "_SC_2_UPE", (long)_SC_2_UPE },
	{ "_SC_2_VERSION", (long)_SC_2_VERSION },
	{ "_SC_ADVISORY_INFO", (long)_SC_ADVISORY_INFO },
	{ "_SC_AIO_LISTIO_MAX", (long)_SC_AIO_LISTIO_MAX },
	{ "_SC_AIO_MAX", (long)_SC_AIO_MAX },
	{ "_SC_AIO_PRIO_DELTA_MAX", (long)_SC_AIO_PRIO_DELTA_MAX },
	{ "_SC_ARG_MAX", (long)_SC_ARG_MAX },
	{ "_SC_ASYNCHRONOUS_IO", (long)_SC_ASYNCHRONOUS_IO },
	{ "_SC_ATEXIT_MAX", (long)_SC_ATEXIT_MAX },
	{ "_SC_BARRIERS", (long)_SC_BARRIERS },
	{ "_SC_BC_BASE_MAX", (long)_SC_BC_BASE_MAX },
	{ "_SC_BC_DIM_MAX", (long)_SC_BC_DIM_MAX },
	{ "_SC_BC_SCALE_MAX", (long)_SC_BC_SCALE_MAX },
	{ "_SC_BC_STRING_MAX", (long)_SC_BC_STRING_MAX },
	{ "_SC_CHILD_MAX", (long)_SC_CHILD_MAX },
	{ "_SC_CLK_TCK", (long)_SC_CLK_TCK },
	{ "_SC_CLOCK_SELECTION", (long)_SC_CLOCK_SELECTION },
	{ "_SC_COLL_WEIGHTS_MAX", (long)_SC_COLL_WEIGHTS_MAX },
	{ "_SC_CPUTIME", (long)_SC_CPUTIME },
	{ "_SC_DELAYTIMER_MAX", (long)_SC_DELAYTIMER_MAX },
	{ "_SC_EXPR_NEST_MAX", (long)_SC_EXPR_NEST_MAX },
	{ "_SC_FSYNC", (long)_SC_FSYNC },
	{ "_SC_GETGR_R_SIZE_MAX", (long)_SC_GETGR_R_SIZE_MAX },
	{ "_SC_GETPW_R_SIZE_MAX", (long)_SC_GETPW_R_SIZE_MAX },
	{ "_SC_HOST_NAME_MAX", (long)_SC_HOST_NAME_MAX },
	{ "_SC_IOV_MAX", (long)_SC_IOV_MAX },
	{ "_SC_IPV6", (long)_SC_IPV6 },
	{ "_SC_JOB_CONTROL", (long)_SC_JOB_CONTROL },
	{ "_SC_LINE_MAX", (long)_SC_LINE_MAX },
	{ "_SC_LOGIN_NAME_MAX", (long)_SC_LOGIN_NAME_MAX },
	{ "_SC_MAPPED_FILES", (long)_SC_MAPPED_FILES },
	{ "_SC_MEMLOCK", (long)_SC_MEMLOCK },
	{ "_SC_MEMLOCK_RANGE", (long)_SC_MEMLOCK_RANGE },
	{ "_SC_MEMORY_PROTECTION", (long)_SC_MEMORY_PROTECTION },
	{ "_SC_MESSAGE_PASSING", (long)_SC_MESSAGE_PASSING },
	{ "_SC_MONOTONIC_CLOCK", (long)_SC_MONOTONIC_CLOCK },
	{ "_SC_MQ_OPEN_MAX", (long)_SC_MQ_OPEN_MAX },
	{ "_SC_MQ_PRIO_MAX", (long)_SC_MQ_PRIO_MAX },
	{ "_SC_NGROUPS_MAX", (long)_SC_NGROUPS_MAX },
	{ "_SC_OPEN_MAX", (long)_SC_OPEN_MAX },
	{ "_SC_PAGE_SIZE", (long)_SC_PAGE_SIZE },
	{ "_SC_PAGESIZE", (long)_SC_PAGESIZE },
	{ "_SC_PRIORITIZED_IO", (long)_SC_PRIORITIZED_IO },
	{ "_SC_PRIORITY_SCHEDULING", (long)_SC_PRIORITY_SCHEDULING },
	{ "_SC_RAW_SOCKETS", (long)_SC_RAW_SOCKETS },
	{ "_SC_RE_DUP_MAX", (long)_SC_RE_DUP_MAX },
	{ "_SC_READER_WRITER_LOCKS", (long)_SC_READER_WRITER_LOCKS },
	{ "_SC_REALTIME_SIGNALS", (long)_SC_REALTIME_SIGNALS },
	{ "_SC_REGEXP", (long)_SC_REGEXP },
	{ "_SC_RTSIG_MAX", (long)_SC_RTSIG_MAX },
	{ "_SC_SAVED_IDS", (long)_SC_SAVED_IDS },
	{ "_SC_SEM_NSEMS_MAX", (long)_SC_SEM_NSEMS_MAX },
	{ "_SC_SEM_VALUE_MAX", (long)_SC_SEM_VALUE_MAX },
	{ "_SC_SEMAPHORES", (long)_SC_SEMAPHORES },
	{ "_SC_SHARED_MEMORY_OBJECTS", (long)_SC_SHARED_MEMORY_OBJECTS },
	{ "_SC_SHELL", (long)_SC_SHELL },
	{ "_SC_SIGQUEUE_MAX", (long)_SC_SIGQUEUE_MAX },
	{ "_SC_SPAWN", (long)_SC_SPAWN },
	{ "_SC_SPIN_LOCKS", (long)_SC_SPIN_LOCKS },
	{ "_SC_SPORADIC_SERVER", (long)_SC_SPORADIC_SERVER },
	{ "_SC_SS_REPL_MAX", (long)_SC_SS_REPL_MAX },
	{ "_SC_STREAM_MAX", (long)_SC_STREAM_MAX },
	{ "_SC_SYMLOOP_MAX", (long)_SC_SYMLOOP_MAX },
	{ "_SC_SYNCHRONIZED_IO", (long)_SC_SYNCHRONIZED_IO },
	{ "_SC_THREAD_ATTR_STACKADDR", (long)_SC_THREAD_ATTR_STACKADDR },
	{ "_SC_THREAD_ATTR_STACKSIZE", (long)_SC_THREAD_ATTR_STACKSIZE },
	{ "_SC_THREAD_CPUTIME", (long)_SC_THREAD_CPUTIME },
	{ "_SC_THREAD_DESTRUCTOR_ITERATIONS", (long)_SC_THREAD_DESTRUCTOR_ITERATIONS },
	{ "_SC_THREAD_KEYS_MAX", (long)_SC_THREAD_KEYS_MAX },
	{ "_SC_THREAD_PRIO_INHERIT", (long)_SC_THREAD_PRIO_INHERIT },
	{ "_SC_THREAD_PRIO_PROTECT", (long)_SC_THREAD_PRIO_PROTECT },
	{ "_SC_THREAD_PRIORITY_SCHEDULING", (long)_SC_THREAD_PRIORITY_SCHEDULING },
	{ "_SC_THREAD_PROCESS_SHARED", (long)_SC_THREAD_PROCESS_SHARED },
	{ "_SC_THREAD_ROBUST_PRIO_INHERIT", (long)_SC_THREAD_ROBUST_PRIO_INHERIT },
	{ "_SC_THREAD_ROBUST_PRIO_PROTECT", (long)_SC_THREAD_ROBUST_PRIO_PROTECT },
	{ "_SC_THREAD_SAFE_FUNCTIONS", (long)_SC_THREAD_SAFE_FUNCTIONS },
	{ "_SC_THREAD_SPORADIC_SERVER", (long)_SC_THREAD_SPORADIC_SERVER },
	{ "_SC_THREAD_STACK_MIN", (long)_SC_THREAD_STACK_MIN },
	{ "_SC_THREAD_THREADS_MAX", (long)_SC_THREAD_THREADS_MAX },
	{ "_SC_THREADS", (long)_SC_THREADS },
	{ "_SC_TIMEOUTS", (long)_SC_TIMEOUTS },
	{ "_SC_TIMER_MAX", (long)_SC_TIMER_MAX },
	{ "_SC_TIMERS", (long)_SC_TIMERS },
	{ "_SC_TTY_NAME_MAX", (long)_SC_TTY_NAME_MAX },
	{ "_SC_TYPED_MEMORY_OBJECTS", (long)_SC_TYPED_MEMORY_OBJECTS },
	{ "_SC_TZNAME_MAX", (long)_SC_TZNAME_MAX },
	{ "_SC_V7_ILP32_OFF32", (long)_SC_V7_ILP32_OFF32 },
	{ "_SC_V7_ILP32_OFFBIG", (long)_SC_V7_ILP32_OFFBIG },
	{ "_SC_V7_LP64_OFF64", (long)_SC_V7_LP64_OFF64 },
	{ "_SC_V7_LPBIG_OFFBIG", (long)_SC_V7_LPBIG_OFFBIG },
	{ "_SC_VERSION", (long)_SC_VERSION },
	{ "_SC_XOPEN_CRYPT", (long)_SC_XOPEN_CRYPT },
	{ "_SC_XOPEN_ENH_I18N", (long)_SC_XOPEN_ENH_I18N },
	{ "_SC_XOPEN_REALTIME", (long)_SC_XOPEN_REALTIME },
	{ "_SC_XOPEN_REALTIME_THREADS", (long)_SC_XOPEN_REALTIME_THREADS },
	{ "_SC_XOPEN_SHM", (long)_SC_XOPEN_SHM },
	{ "_SC_XOPEN_UNIX", (long)_SC_XOPEN_UNIX },
	{ "_SC_XOPEN_UUCP", (long)_SC_XOPEN_UUCP },
	{ "_SC_XOPEN_VERSION", (long)_SC_XOPEN_VERSION },
};
static const struct c new_PC[] = {
	{ "_PC_FALLOC", (long)_PC_FALLOC },
	{ "_PC_TEXTDOMAIN_MAX", (long)_PC_TEXTDOMAIN_MAX },
};
static const struct c old_PC[] = {
	{ "_PC_2_SYMLINKS", (long)_PC_2_SYMLINKS },
	{ "_PC_ALLOC_SIZE_MIN", (long)_PC_ALLOC_SIZE_MIN },
	{ "_PC_ASYNC_IO", (long)_PC_ASYNC_IO },
	{ "_PC_CHOWN_RESTRICTED", (long)_PC_CHOWN_RESTRICTED },
	{ "_PC_FILESIZEBITS", (long)_PC_FILESIZEBITS },
	{ "_PC_LINK_MAX", (long)_PC_LINK_MAX },
	{ "_PC_MAX_CANON", (long)_PC_MAX_CANON },
	{ "_PC_MAX_INPUT", (long)_PC_MAX_INPUT },
	{ "_PC_NAME_MAX", (long)_PC_NAME_MAX },
	{ "_PC_NO_TRUNC", (long)_PC_NO_TRUNC },
	{ "_PC_PATH_MAX", (long)_PC_PATH_MAX },
	{ "_PC_PIPE_BUF", (long)_PC_PIPE_BUF },
	{ "_PC_PRIO_IO", (long)_PC_PRIO_IO },
	{ "_PC_REC_INCR_XFER_SIZE", (long)_PC_REC_INCR_XFER_SIZE },
	{ "_PC_REC_MAX_XFER_SIZE", (long)_PC_REC_MAX_XFER_SIZE },
	{ "_PC_REC_MIN_XFER_SIZE", (long)_PC_REC_MIN_XFER_SIZE },
	{ "_PC_REC_XFER_ALIGN", (long)_PC_REC_XFER_ALIGN },
	{ "_PC_SYMLINK_MAX", (long)_PC_SYMLINK_MAX },
	{ "_PC_SYNC_IO", (long)_PC_SYNC_IO },
	{ "_PC_TIMESTAMP_RESOLUTION", (long)_PC_TIMESTAMP_RESOLUTION },
	{ "_PC_VDISABLE", (long)_PC_VDISABLE },
};
static const struct c new_CS[] = {
	{ "_CS_POSIX_V8_ILP32_OFF32_CFLAGS", (long)_CS_POSIX_V8_ILP32_OFF32_CFLAGS },
	{ "_CS_POSIX_V8_ILP32_OFF32_LDFLAGS", (long)_CS_POSIX_V8_ILP32_OFF32_LDFLAGS },
	{ "_CS_POSIX_V8_ILP32_OFF32_LIBS", (long)_CS_POSIX_V8_ILP32_OFF32_LIBS },
	{ "_CS_POSIX_V8_ILP32_OFFBIG_CFLAGS", (long)_CS_POSIX_V8_ILP32_OFFBIG_CFLAGS },
	{ "_CS_POSIX_V8_ILP32_OFFBIG_LDFLAGS", (long)_CS_POSIX_V8_ILP32_OFFBIG_LDFLAGS },
	{ "_CS_POSIX_V8_ILP32_OFFBIG_LIBS", (long)_CS_POSIX_V8_ILP32_OFFBIG_LIBS },
	{ "_CS_POSIX_V8_LP64_OFF64_CFLAGS", (long)_CS_POSIX_V8_LP64_OFF64_CFLAGS },
	{ "_CS_POSIX_V8_LP64_OFF64_LDFLAGS", (long)_CS_POSIX_V8_LP64_OFF64_LDFLAGS },
	{ "_CS_POSIX_V8_LP64_OFF64_LIBS", (long)_CS_POSIX_V8_LP64_OFF64_LIBS },
	{ "_CS_POSIX_V8_LPBIG_OFFBIG_CFLAGS", (long)_CS_POSIX_V8_LPBIG_OFFBIG_CFLAGS },
	{ "_CS_POSIX_V8_LPBIG_OFFBIG_LDFLAGS", (long)_CS_POSIX_V8_LPBIG_OFFBIG_LDFLAGS },
	{ "_CS_POSIX_V8_LPBIG_OFFBIG_LIBS", (long)_CS_POSIX_V8_LPBIG_OFFBIG_LIBS },
	{ "_CS_POSIX_V8_THREADS_CFLAGS", (long)_CS_POSIX_V8_THREADS_CFLAGS },
	{ "_CS_POSIX_V8_THREADS_LDFLAGS", (long)_CS_POSIX_V8_THREADS_LDFLAGS },
	{ "_CS_POSIX_V8_WIDTH_RESTRICTED_ENVS", (long)_CS_POSIX_V8_WIDTH_RESTRICTED_ENVS },
	{ "_CS_V8_ENV", (long)_CS_V8_ENV },
};
static const struct c old_CS[] = {
	{ "_CS_PATH", (long)_CS_PATH },
#ifdef _CS_POSIX_V7_ILP32_OFF32_CFLAGS
	{ "_CS_POSIX_V7_ILP32_OFF32_CFLAGS", (long)_CS_POSIX_V7_ILP32_OFF32_CFLAGS },
#endif
#ifdef _CS_POSIX_V7_ILP32_OFF32_LDFLAGS
	{ "_CS_POSIX_V7_ILP32_OFF32_LDFLAGS", (long)_CS_POSIX_V7_ILP32_OFF32_LDFLAGS },
#endif
#ifdef _CS_POSIX_V7_ILP32_OFF32_LIBS
	{ "_CS_POSIX_V7_ILP32_OFF32_LIBS", (long)_CS_POSIX_V7_ILP32_OFF32_LIBS },
#endif
#ifdef _CS_POSIX_V7_ILP32_OFFBIG_CFLAGS
	{ "_CS_POSIX_V7_ILP32_OFFBIG_CFLAGS", (long)_CS_POSIX_V7_ILP32_OFFBIG_CFLAGS },
#endif
#ifdef _CS_POSIX_V7_ILP32_OFFBIG_LDFLAGS
	{ "_CS_POSIX_V7_ILP32_OFFBIG_LDFLAGS", (long)_CS_POSIX_V7_ILP32_OFFBIG_LDFLAGS },
#endif
#ifdef _CS_POSIX_V7_ILP32_OFFBIG_LIBS
	{ "_CS_POSIX_V7_ILP32_OFFBIG_LIBS", (long)_CS_POSIX_V7_ILP32_OFFBIG_LIBS },
#endif
#ifdef _CS_POSIX_V7_LP64_OFF64_CFLAGS
	{ "_CS_POSIX_V7_LP64_OFF64_CFLAGS", (long)_CS_POSIX_V7_LP64_OFF64_CFLAGS },
#endif
#ifdef _CS_POSIX_V7_LP64_OFF64_LDFLAGS
	{ "_CS_POSIX_V7_LP64_OFF64_LDFLAGS", (long)_CS_POSIX_V7_LP64_OFF64_LDFLAGS },
#endif
#ifdef _CS_POSIX_V7_LP64_OFF64_LIBS
	{ "_CS_POSIX_V7_LP64_OFF64_LIBS", (long)_CS_POSIX_V7_LP64_OFF64_LIBS },
#endif
#ifdef _CS_POSIX_V7_LPBIG_OFFBIG_CFLAGS
	{ "_CS_POSIX_V7_LPBIG_OFFBIG_CFLAGS", (long)_CS_POSIX_V7_LPBIG_OFFBIG_CFLAGS },
#endif
#ifdef _CS_POSIX_V7_LPBIG_OFFBIG_LDFLAGS
	{ "_CS_POSIX_V7_LPBIG_OFFBIG_LDFLAGS", (long)_CS_POSIX_V7_LPBIG_OFFBIG_LDFLAGS },
#endif
#ifdef _CS_POSIX_V7_LPBIG_OFFBIG_LIBS
	{ "_CS_POSIX_V7_LPBIG_OFFBIG_LIBS", (long)_CS_POSIX_V7_LPBIG_OFFBIG_LIBS },
#endif
#ifdef _CS_POSIX_V7_THREADS_CFLAGS
	{ "_CS_POSIX_V7_THREADS_CFLAGS", (long)_CS_POSIX_V7_THREADS_CFLAGS },
#endif
#ifdef _CS_POSIX_V7_THREADS_LDFLAGS
	{ "_CS_POSIX_V7_THREADS_LDFLAGS", (long)_CS_POSIX_V7_THREADS_LDFLAGS },
#endif
#ifdef _CS_POSIX_V7_WIDTH_RESTRICTED_ENVS
	{ "_CS_POSIX_V7_WIDTH_RESTRICTED_ENVS", (long)_CS_POSIX_V7_WIDTH_RESTRICTED_ENVS },
#endif
#ifdef _CS_V7_ENV
	{ "_CS_V7_ENV", (long)_CS_V7_ENV },
#endif
};

#define ARRAY_LEN(a) (sizeof (a) / sizeof (a)[0])

/* True when two names differ only in V7 against V8: they name the same environment, so one value for both is not excluded. */
static int same_env(const char *a, const char *b)
{
	char x[96], y[96];
	size_t i;

	if (strlen(a) >= sizeof x || strlen(b) >= sizeof y)
		return 0;
	strcpy(x, a);
	strcpy(y, b);
	for (i = 0; x[i] != '\0'; i++)
		if (x[i] == 'V' && x[i + 1] == '7')
			x[i + 1] = '8';
	for (i = 0; y[i] != '\0'; i++)
		if (y[i] == 'V' && y[i + 1] == '7')
			y[i + 1] = '8';
	return strcmp(x, y) == 0;
}

static void against(const struct c *x, size_t nx, const struct c *y, size_t ny, int same)
{
	size_t i, j;

	for (i = 0; i < nx; i++)
		for (j = same ? i + 1 : 0; j < ny; j++)
			pc_check(same_env(x[i].name, y[j].name) || x[i].val != y[j].val, "%s and %s have the same value (%ld)", x[i].name, y[j].name, x[i].val);
}

int main(void)
{
	pc_begin("XBD.unistd.h.019");
	pc_test();
	against(new_SC, ARRAY_LEN(new_SC), new_SC, ARRAY_LEN(new_SC), 1);
	against(new_SC, ARRAY_LEN(new_SC), old_SC, ARRAY_LEN(old_SC), 0);
	against(new_PC, ARRAY_LEN(new_PC), new_PC, ARRAY_LEN(new_PC), 1);
	against(new_PC, ARRAY_LEN(new_PC), old_PC, ARRAY_LEN(old_PC), 0);
	against(new_CS, ARRAY_LEN(new_CS), new_CS, ARRAY_LEN(new_CS), 1);
	against(new_CS, ARRAY_LEN(new_CS), old_CS, ARRAY_LEN(old_CS), 0);
	pc_pass();
}
