/*
 * <pthread.h> functions clock lock
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.pthread.h.015
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
	pc_begin("XBD.pthread.h.015");
	pc_test();
	if (!IS_T(&(pthread_cond_clockwait), int (*)(pthread_cond_t *restrict, pthread_mutex_t *restrict, clockid_t, const struct timespec *restrict)))
		pc_fail("pthread_cond_clockwait() is not declared as int pthread_cond_clockwait(pthread_cond_t *restrict, pthread_mutex_t *restrict, clockid_t, const struct timespec *restrict)");
	if (IS_T(&(pthread_cond_clockwait), int (*)(pthread_cond_t *restrict, pthread_mutex_t *restrict, clockid_t, const struct timespec *restrict, int)))
		pc_fail("pthread_cond_clockwait() is declared without a prototype");
	if (!IS_T(&(pthread_mutex_clocklock), int (*)(pthread_mutex_t *restrict, clockid_t, const struct timespec *restrict)))
		pc_fail("pthread_mutex_clocklock() is not declared as int pthread_mutex_clocklock(pthread_mutex_t *restrict, clockid_t, const struct timespec *restrict)");
	if (IS_T(&(pthread_mutex_clocklock), int (*)(pthread_mutex_t *restrict, clockid_t, const struct timespec *restrict, int)))
		pc_fail("pthread_mutex_clocklock() is declared without a prototype");
	if (!IS_T(&(pthread_rwlock_clockrdlock), int (*)(pthread_rwlock_t *restrict, clockid_t, const struct timespec *restrict)))
		pc_fail("pthread_rwlock_clockrdlock() is not declared as int pthread_rwlock_clockrdlock(pthread_rwlock_t *restrict, clockid_t, const struct timespec *restrict)");
	if (IS_T(&(pthread_rwlock_clockrdlock), int (*)(pthread_rwlock_t *restrict, clockid_t, const struct timespec *restrict, int)))
		pc_fail("pthread_rwlock_clockrdlock() is declared without a prototype");
	if (!IS_T(&(pthread_rwlock_clockwrlock), int (*)(pthread_rwlock_t *restrict, clockid_t, const struct timespec *restrict)))
		pc_fail("pthread_rwlock_clockwrlock() is not declared as int pthread_rwlock_clockwrlock(pthread_rwlock_t *restrict, clockid_t, const struct timespec *restrict)");
	if (IS_T(&(pthread_rwlock_clockwrlock), int (*)(pthread_rwlock_t *restrict, clockid_t, const struct timespec *restrict, int)))
		pc_fail("pthread_rwlock_clockwrlock() is declared without a prototype");
	pc_pass();
}
