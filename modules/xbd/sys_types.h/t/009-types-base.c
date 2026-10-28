/*
 * <sys/types.h> types base
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sys_types.h.009
 *
 * Only <sys/types.h> is included (not pc.h, which brings in <stddef.h>), so a name
 * the header fails to provide stops the build, which the module reports as FAIL.
 */
#include <sys/types.h>

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

typedef blkcnt_t *pc_t0;
typedef blksize_t *pc_t1;
typedef clock_t *pc_t2;
typedef clockid_t *pc_t3;
typedef dev_t *pc_t4;
typedef fsblkcnt_t *pc_t5;
typedef fsfilcnt_t *pc_t6;
typedef gid_t *pc_t7;
typedef id_t *pc_t8;
typedef ino_t *pc_t9;
typedef mode_t *pc_t10;
typedef nlink_t *pc_t11;
typedef off_t *pc_t12;
typedef pid_t *pc_t13;
typedef pthread_attr_t *pc_t14;
typedef pthread_barrier_t *pc_t15;
typedef pthread_barrierattr_t *pc_t16;
typedef pthread_cond_t *pc_t17;
typedef pthread_condattr_t *pc_t18;
typedef pthread_key_t *pc_t19;
typedef pthread_mutex_t *pc_t20;
typedef pthread_mutexattr_t *pc_t21;
typedef pthread_once_t *pc_t22;
typedef pthread_rwlock_t *pc_t23;
typedef pthread_rwlockattr_t *pc_t24;
typedef pthread_spinlock_t *pc_t25;
typedef pthread_t *pc_t26;
typedef size_t *pc_t27;
typedef ssize_t *pc_t28;
typedef suseconds_t *pc_t29;
typedef time_t *pc_t30;
typedef timer_t *pc_t31;
typedef uid_t *pc_t32;

int main(void)
{
	pc_begin("XBD.sys_types.h.009");
	pc_test();
	pc_pass();
}
