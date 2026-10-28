/*
 * <pthread.h> functions tsh
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.pthread.h.022
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

int main(void)
{
	pc_begin("XBD.pthread.h.022");
	pc_test();
	if (!IS_T(&(pthread_barrierattr_getpshared), int (*)( const pthread_barrierattr_t *restrict, int *restrict)))
		pc_fail("pthread_barrierattr_getpshared() is not declared as int pthread_barrierattr_getpshared( const pthread_barrierattr_t *restrict, int *restrict)");
	if (IS_T(&(pthread_barrierattr_getpshared), int (*)(const pthread_barrierattr_t *restrict, int *restrict, int)))
		pc_fail("pthread_barrierattr_getpshared() is declared without a prototype");
	if (!IS_T(&(pthread_barrierattr_setpshared), int (*)(pthread_barrierattr_t *, int)))
		pc_fail("pthread_barrierattr_setpshared() is not declared as int pthread_barrierattr_setpshared(pthread_barrierattr_t *, int)");
	if (IS_T(&(pthread_barrierattr_setpshared), int (*)(pthread_barrierattr_t *, int, int)))
		pc_fail("pthread_barrierattr_setpshared() is declared without a prototype");
	if (!IS_T(&(pthread_condattr_getpshared), int (*)(const pthread_condattr_t *restrict, int *restrict)))
		pc_fail("pthread_condattr_getpshared() is not declared as int pthread_condattr_getpshared(const pthread_condattr_t *restrict, int *restrict)");
	if (IS_T(&(pthread_condattr_getpshared), int (*)(const pthread_condattr_t *restrict, int *restrict, int)))
		pc_fail("pthread_condattr_getpshared() is declared without a prototype");
	if (!IS_T(&(pthread_condattr_setpshared), int (*)(pthread_condattr_t *, int)))
		pc_fail("pthread_condattr_setpshared() is not declared as int pthread_condattr_setpshared(pthread_condattr_t *, int)");
	if (IS_T(&(pthread_condattr_setpshared), int (*)(pthread_condattr_t *, int, int)))
		pc_fail("pthread_condattr_setpshared() is declared without a prototype");
	if (!IS_T(&(pthread_mutexattr_getpshared), int (*)(const pthread_mutexattr_t *restrict, int *restrict)))
		pc_fail("pthread_mutexattr_getpshared() is not declared as int pthread_mutexattr_getpshared(const pthread_mutexattr_t *restrict, int *restrict)");
	if (IS_T(&(pthread_mutexattr_getpshared), int (*)(const pthread_mutexattr_t *restrict, int *restrict, int)))
		pc_fail("pthread_mutexattr_getpshared() is declared without a prototype");
	if (!IS_T(&(pthread_mutexattr_setpshared), int (*)(pthread_mutexattr_t *, int)))
		pc_fail("pthread_mutexattr_setpshared() is not declared as int pthread_mutexattr_setpshared(pthread_mutexattr_t *, int)");
	if (IS_T(&(pthread_mutexattr_setpshared), int (*)(pthread_mutexattr_t *, int, int)))
		pc_fail("pthread_mutexattr_setpshared() is declared without a prototype");
	if (!IS_T(&(pthread_rwlockattr_getpshared), int (*)( const pthread_rwlockattr_t *restrict, int *restrict)))
		pc_fail("pthread_rwlockattr_getpshared() is not declared as int pthread_rwlockattr_getpshared( const pthread_rwlockattr_t *restrict, int *restrict)");
	if (IS_T(&(pthread_rwlockattr_getpshared), int (*)(const pthread_rwlockattr_t *restrict, int *restrict, int)))
		pc_fail("pthread_rwlockattr_getpshared() is declared without a prototype");
	if (!IS_T(&(pthread_rwlockattr_setpshared), int (*)(pthread_rwlockattr_t *, int)))
		pc_fail("pthread_rwlockattr_setpshared() is not declared as int pthread_rwlockattr_setpshared(pthread_rwlockattr_t *, int)");
	if (IS_T(&(pthread_rwlockattr_setpshared), int (*)(pthread_rwlockattr_t *, int, int)))
		pc_fail("pthread_rwlockattr_setpshared() is declared without a prototype");
	pc_pass();
}
