/*
 * <pthread.h> types base
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.pthread.h.014
 *
 * Only <pthread.h> is included (not pc.h, which brings in <stddef.h>), so a name
 * the header fails to provide stops the build, which the module reports as FAIL.
 */
#include <pthread.h>

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

typedef pthread_attr_t *pc_t0;
typedef pthread_barrier_t *pc_t1;
typedef pthread_barrierattr_t *pc_t2;
typedef pthread_cond_t *pc_t3;
typedef pthread_condattr_t *pc_t4;
typedef pthread_key_t *pc_t5;
typedef pthread_mutex_t *pc_t6;
typedef pthread_mutexattr_t *pc_t7;
typedef pthread_once_t *pc_t8;
typedef pthread_rwlock_t *pc_t9;
typedef pthread_rwlockattr_t *pc_t10;
typedef pthread_spinlock_t *pc_t11;
typedef pthread_t *pc_t12;

int main(void)
{
	pc_begin("XBD.pthread.h.014");
	pc_test();
	pc_pass();
}
