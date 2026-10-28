/*
 * <pthread.h> functions
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.pthread.h.016
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
	pc_begin("XBD.pthread.h.016");
	pc_test();
	if (!IS_T(&(pthread_atfork), int (*)(void (*)(void), void (*)(void), void(*)(void))))
		pc_fail("pthread_atfork() is not declared as int pthread_atfork(void (*)(void), void (*)(void), void(*)(void))");
	if (IS_T(&(pthread_atfork), int (*)(void (*)(void), void (*)(void), void(*)(void), int)))
		pc_fail("pthread_atfork() is declared without a prototype");
	if (!IS_T(&(pthread_attr_destroy), int (*)(pthread_attr_t *)))
		pc_fail("pthread_attr_destroy() is not declared as int pthread_attr_destroy(pthread_attr_t *)");
	if (IS_T(&(pthread_attr_destroy), int (*)(pthread_attr_t *, int)))
		pc_fail("pthread_attr_destroy() is declared without a prototype");
	if (!IS_T(&(pthread_attr_getdetachstate), int (*)(const pthread_attr_t *, int *)))
		pc_fail("pthread_attr_getdetachstate() is not declared as int pthread_attr_getdetachstate(const pthread_attr_t *, int *)");
	if (IS_T(&(pthread_attr_getdetachstate), int (*)(const pthread_attr_t *, int *, int)))
		pc_fail("pthread_attr_getdetachstate() is declared without a prototype");
	if (!IS_T(&(pthread_attr_getguardsize), int (*)(const pthread_attr_t *restrict, size_t *restrict)))
		pc_fail("pthread_attr_getguardsize() is not declared as int pthread_attr_getguardsize(const pthread_attr_t *restrict, size_t *restrict)");
	if (IS_T(&(pthread_attr_getguardsize), int (*)(const pthread_attr_t *restrict, size_t *restrict, int)))
		pc_fail("pthread_attr_getguardsize() is declared without a prototype");
	if (!IS_T(&(pthread_attr_getschedparam), int (*)(const pthread_attr_t *restrict, struct sched_param *restrict)))
		pc_fail("pthread_attr_getschedparam() is not declared as int pthread_attr_getschedparam(const pthread_attr_t *restrict, struct sched_param *restrict)");
	if (IS_T(&(pthread_attr_getschedparam), int (*)(const pthread_attr_t *restrict, struct sched_param *restrict, int)))
		pc_fail("pthread_attr_getschedparam() is declared without a prototype");
	if (!IS_T(&(pthread_attr_init), int (*)(pthread_attr_t *)))
		pc_fail("pthread_attr_init() is not declared as int pthread_attr_init(pthread_attr_t *)");
	if (IS_T(&(pthread_attr_init), int (*)(pthread_attr_t *, int)))
		pc_fail("pthread_attr_init() is declared without a prototype");
	if (!IS_T(&(pthread_attr_setdetachstate), int (*)(pthread_attr_t *, int)))
		pc_fail("pthread_attr_setdetachstate() is not declared as int pthread_attr_setdetachstate(pthread_attr_t *, int)");
	if (IS_T(&(pthread_attr_setdetachstate), int (*)(pthread_attr_t *, int, int)))
		pc_fail("pthread_attr_setdetachstate() is declared without a prototype");
	if (!IS_T(&(pthread_attr_setguardsize), int (*)(pthread_attr_t *, size_t)))
		pc_fail("pthread_attr_setguardsize() is not declared as int pthread_attr_setguardsize(pthread_attr_t *, size_t)");
	if (IS_T(&(pthread_attr_setguardsize), int (*)(pthread_attr_t *, size_t, int)))
		pc_fail("pthread_attr_setguardsize() is declared without a prototype");
	if (!IS_T(&(pthread_attr_setschedparam), int (*)(pthread_attr_t *restrict, const struct sched_param *restrict)))
		pc_fail("pthread_attr_setschedparam() is not declared as int pthread_attr_setschedparam(pthread_attr_t *restrict, const struct sched_param *restrict)");
	if (IS_T(&(pthread_attr_setschedparam), int (*)(pthread_attr_t *restrict, const struct sched_param *restrict, int)))
		pc_fail("pthread_attr_setschedparam() is declared without a prototype");
	if (!IS_T(&(pthread_barrier_destroy), int (*)(pthread_barrier_t *)))
		pc_fail("pthread_barrier_destroy() is not declared as int pthread_barrier_destroy(pthread_barrier_t *)");
	if (IS_T(&(pthread_barrier_destroy), int (*)(pthread_barrier_t *, int)))
		pc_fail("pthread_barrier_destroy() is declared without a prototype");
	if (!IS_T(&(pthread_barrier_init), int (*)(pthread_barrier_t *restrict, const pthread_barrierattr_t *restrict, unsigned)))
		pc_fail("pthread_barrier_init() is not declared as int pthread_barrier_init(pthread_barrier_t *restrict, const pthread_barrierattr_t *restrict, unsigned)");
	if (IS_T(&(pthread_barrier_init), int (*)(pthread_barrier_t *restrict, const pthread_barrierattr_t *restrict, unsigned, int)))
		pc_fail("pthread_barrier_init() is declared without a prototype");
	if (!IS_T(&(pthread_barrier_wait), int (*)(pthread_barrier_t *)))
		pc_fail("pthread_barrier_wait() is not declared as int pthread_barrier_wait(pthread_barrier_t *)");
	if (IS_T(&(pthread_barrier_wait), int (*)(pthread_barrier_t *, int)))
		pc_fail("pthread_barrier_wait() is declared without a prototype");
	if (!IS_T(&(pthread_barrierattr_destroy), int (*)(pthread_barrierattr_t *)))
		pc_fail("pthread_barrierattr_destroy() is not declared as int pthread_barrierattr_destroy(pthread_barrierattr_t *)");
	if (IS_T(&(pthread_barrierattr_destroy), int (*)(pthread_barrierattr_t *, int)))
		pc_fail("pthread_barrierattr_destroy() is declared without a prototype");
	if (!IS_T(&(pthread_barrierattr_init), int (*)(pthread_barrierattr_t *)))
		pc_fail("pthread_barrierattr_init() is not declared as int pthread_barrierattr_init(pthread_barrierattr_t *)");
	if (IS_T(&(pthread_barrierattr_init), int (*)(pthread_barrierattr_t *, int)))
		pc_fail("pthread_barrierattr_init() is declared without a prototype");
	if (!IS_T(&(pthread_cancel), int (*)(pthread_t)))
		pc_fail("pthread_cancel() is not declared as int pthread_cancel(pthread_t)");
	if (IS_T(&(pthread_cancel), int (*)(pthread_t, int)))
		pc_fail("pthread_cancel() is declared without a prototype");
	if (!IS_T(&(pthread_cond_broadcast), int (*)(pthread_cond_t *)))
		pc_fail("pthread_cond_broadcast() is not declared as int pthread_cond_broadcast(pthread_cond_t *)");
	if (IS_T(&(pthread_cond_broadcast), int (*)(pthread_cond_t *, int)))
		pc_fail("pthread_cond_broadcast() is declared without a prototype");
	if (!IS_T(&(pthread_cond_destroy), int (*)(pthread_cond_t *)))
		pc_fail("pthread_cond_destroy() is not declared as int pthread_cond_destroy(pthread_cond_t *)");
	if (IS_T(&(pthread_cond_destroy), int (*)(pthread_cond_t *, int)))
		pc_fail("pthread_cond_destroy() is declared without a prototype");
	if (!IS_T(&(pthread_cond_init), int (*)(pthread_cond_t *restrict, const pthread_condattr_t *restrict)))
		pc_fail("pthread_cond_init() is not declared as int pthread_cond_init(pthread_cond_t *restrict, const pthread_condattr_t *restrict)");
	if (IS_T(&(pthread_cond_init), int (*)(pthread_cond_t *restrict, const pthread_condattr_t *restrict, int)))
		pc_fail("pthread_cond_init() is declared without a prototype");
	if (!IS_T(&(pthread_cond_signal), int (*)(pthread_cond_t *)))
		pc_fail("pthread_cond_signal() is not declared as int pthread_cond_signal(pthread_cond_t *)");
	if (IS_T(&(pthread_cond_signal), int (*)(pthread_cond_t *, int)))
		pc_fail("pthread_cond_signal() is declared without a prototype");
	if (!IS_T(&(pthread_cond_timedwait), int (*)(pthread_cond_t *restrict, pthread_mutex_t *restrict, const struct timespec *restrict)))
		pc_fail("pthread_cond_timedwait() is not declared as int pthread_cond_timedwait(pthread_cond_t *restrict, pthread_mutex_t *restrict, const struct timespec *restrict)");
	if (IS_T(&(pthread_cond_timedwait), int (*)(pthread_cond_t *restrict, pthread_mutex_t *restrict, const struct timespec *restrict, int)))
		pc_fail("pthread_cond_timedwait() is declared without a prototype");
	if (!IS_T(&(pthread_cond_wait), int (*)(pthread_cond_t *restrict, pthread_mutex_t *restrict)))
		pc_fail("pthread_cond_wait() is not declared as int pthread_cond_wait(pthread_cond_t *restrict, pthread_mutex_t *restrict)");
	if (IS_T(&(pthread_cond_wait), int (*)(pthread_cond_t *restrict, pthread_mutex_t *restrict, int)))
		pc_fail("pthread_cond_wait() is declared without a prototype");
	if (!IS_T(&(pthread_condattr_destroy), int (*)(pthread_condattr_t *)))
		pc_fail("pthread_condattr_destroy() is not declared as int pthread_condattr_destroy(pthread_condattr_t *)");
	if (IS_T(&(pthread_condattr_destroy), int (*)(pthread_condattr_t *, int)))
		pc_fail("pthread_condattr_destroy() is declared without a prototype");
	if (!IS_T(&(pthread_condattr_getclock), int (*)(const pthread_condattr_t *restrict, clockid_t *restrict)))
		pc_fail("pthread_condattr_getclock() is not declared as int pthread_condattr_getclock(const pthread_condattr_t *restrict, clockid_t *restrict)");
	if (IS_T(&(pthread_condattr_getclock), int (*)(const pthread_condattr_t *restrict, clockid_t *restrict, int)))
		pc_fail("pthread_condattr_getclock() is declared without a prototype");
	if (!IS_T(&(pthread_condattr_init), int (*)(pthread_condattr_t *)))
		pc_fail("pthread_condattr_init() is not declared as int pthread_condattr_init(pthread_condattr_t *)");
	if (IS_T(&(pthread_condattr_init), int (*)(pthread_condattr_t *, int)))
		pc_fail("pthread_condattr_init() is declared without a prototype");
	if (!IS_T(&(pthread_condattr_setclock), int (*)(pthread_condattr_t *, clockid_t)))
		pc_fail("pthread_condattr_setclock() is not declared as int pthread_condattr_setclock(pthread_condattr_t *, clockid_t)");
	if (IS_T(&(pthread_condattr_setclock), int (*)(pthread_condattr_t *, clockid_t, int)))
		pc_fail("pthread_condattr_setclock() is declared without a prototype");
	if (!IS_T(&(pthread_create), int (*)(pthread_t *restrict, const pthread_attr_t *restrict, void *(*)(void*), void *restrict)))
		pc_fail("pthread_create() is not declared as int pthread_create(pthread_t *restrict, const pthread_attr_t *restrict, void *(*)(void*), void *restrict)");
	if (IS_T(&(pthread_create), int (*)(pthread_t *restrict, const pthread_attr_t *restrict, void *(*)(void*), void *restrict, int)))
		pc_fail("pthread_create() is declared without a prototype");
	if (!IS_T(&(pthread_detach), int (*)(pthread_t)))
		pc_fail("pthread_detach() is not declared as int pthread_detach(pthread_t)");
	if (IS_T(&(pthread_detach), int (*)(pthread_t, int)))
		pc_fail("pthread_detach() is declared without a prototype");
	if (!IS_T(&(pthread_equal), int (*)(pthread_t, pthread_t)))
		pc_fail("pthread_equal() is not declared as int pthread_equal(pthread_t, pthread_t)");
	if (IS_T(&(pthread_equal), int (*)(pthread_t, pthread_t, int)))
		pc_fail("pthread_equal() is declared without a prototype");
	if (!IS_T(&(pthread_exit), void (*)(void *)))
		pc_fail("pthread_exit() is not declared as _Noreturn void pthread_exit(void *)");
	if (IS_T(&(pthread_exit), void (*)(void *, int)))
		pc_fail("pthread_exit() is declared without a prototype");
	if (!IS_T(&(pthread_getspecific), void *(*)(pthread_key_t)))
		pc_fail("pthread_getspecific() is not declared as void *pthread_getspecific(pthread_key_t)");
	if (IS_T(&(pthread_getspecific), void *(*)(pthread_key_t, int)))
		pc_fail("pthread_getspecific() is declared without a prototype");
	if (!IS_T(&(pthread_join), int (*)(pthread_t, void **)))
		pc_fail("pthread_join() is not declared as int pthread_join(pthread_t, void **)");
	if (IS_T(&(pthread_join), int (*)(pthread_t, void **, int)))
		pc_fail("pthread_join() is declared without a prototype");
	if (!IS_T(&(pthread_key_create), int (*)(pthread_key_t *, void (*)(void*))))
		pc_fail("pthread_key_create() is not declared as int pthread_key_create(pthread_key_t *, void (*)(void*))");
	if (IS_T(&(pthread_key_create), int (*)(pthread_key_t *, void (*)(void*), int)))
		pc_fail("pthread_key_create() is declared without a prototype");
	if (!IS_T(&(pthread_key_delete), int (*)(pthread_key_t)))
		pc_fail("pthread_key_delete() is not declared as int pthread_key_delete(pthread_key_t)");
	if (IS_T(&(pthread_key_delete), int (*)(pthread_key_t, int)))
		pc_fail("pthread_key_delete() is declared without a prototype");
	if (!IS_T(&(pthread_mutex_consistent), int (*)(pthread_mutex_t *)))
		pc_fail("pthread_mutex_consistent() is not declared as int pthread_mutex_consistent(pthread_mutex_t *)");
	if (IS_T(&(pthread_mutex_consistent), int (*)(pthread_mutex_t *, int)))
		pc_fail("pthread_mutex_consistent() is declared without a prototype");
	if (!IS_T(&(pthread_mutex_destroy), int (*)(pthread_mutex_t *)))
		pc_fail("pthread_mutex_destroy() is not declared as int pthread_mutex_destroy(pthread_mutex_t *)");
	if (IS_T(&(pthread_mutex_destroy), int (*)(pthread_mutex_t *, int)))
		pc_fail("pthread_mutex_destroy() is declared without a prototype");
	if (!IS_T(&(pthread_mutex_init), int (*)(pthread_mutex_t *restrict, const pthread_mutexattr_t *restrict)))
		pc_fail("pthread_mutex_init() is not declared as int pthread_mutex_init(pthread_mutex_t *restrict, const pthread_mutexattr_t *restrict)");
	if (IS_T(&(pthread_mutex_init), int (*)(pthread_mutex_t *restrict, const pthread_mutexattr_t *restrict, int)))
		pc_fail("pthread_mutex_init() is declared without a prototype");
	if (!IS_T(&(pthread_mutex_lock), int (*)(pthread_mutex_t *)))
		pc_fail("pthread_mutex_lock() is not declared as int pthread_mutex_lock(pthread_mutex_t *)");
	if (IS_T(&(pthread_mutex_lock), int (*)(pthread_mutex_t *, int)))
		pc_fail("pthread_mutex_lock() is declared without a prototype");
	if (!IS_T(&(pthread_mutex_timedlock), int (*)(pthread_mutex_t *restrict, const struct timespec *restrict)))
		pc_fail("pthread_mutex_timedlock() is not declared as int pthread_mutex_timedlock(pthread_mutex_t *restrict, const struct timespec *restrict)");
	if (IS_T(&(pthread_mutex_timedlock), int (*)(pthread_mutex_t *restrict, const struct timespec *restrict, int)))
		pc_fail("pthread_mutex_timedlock() is declared without a prototype");
	if (!IS_T(&(pthread_mutex_trylock), int (*)(pthread_mutex_t *)))
		pc_fail("pthread_mutex_trylock() is not declared as int pthread_mutex_trylock(pthread_mutex_t *)");
	if (IS_T(&(pthread_mutex_trylock), int (*)(pthread_mutex_t *, int)))
		pc_fail("pthread_mutex_trylock() is declared without a prototype");
	if (!IS_T(&(pthread_mutex_unlock), int (*)(pthread_mutex_t *)))
		pc_fail("pthread_mutex_unlock() is not declared as int pthread_mutex_unlock(pthread_mutex_t *)");
	if (IS_T(&(pthread_mutex_unlock), int (*)(pthread_mutex_t *, int)))
		pc_fail("pthread_mutex_unlock() is declared without a prototype");
	if (!IS_T(&(pthread_mutexattr_destroy), int (*)(pthread_mutexattr_t *)))
		pc_fail("pthread_mutexattr_destroy() is not declared as int pthread_mutexattr_destroy(pthread_mutexattr_t *)");
	if (IS_T(&(pthread_mutexattr_destroy), int (*)(pthread_mutexattr_t *, int)))
		pc_fail("pthread_mutexattr_destroy() is declared without a prototype");
	if (!IS_T(&(pthread_mutexattr_getrobust), int (*)(const pthread_mutexattr_t *restrict, int *restrict)))
		pc_fail("pthread_mutexattr_getrobust() is not declared as int pthread_mutexattr_getrobust(const pthread_mutexattr_t *restrict, int *restrict)");
	if (IS_T(&(pthread_mutexattr_getrobust), int (*)(const pthread_mutexattr_t *restrict, int *restrict, int)))
		pc_fail("pthread_mutexattr_getrobust() is declared without a prototype");
	if (!IS_T(&(pthread_mutexattr_gettype), int (*)(const pthread_mutexattr_t *restrict, int *restrict)))
		pc_fail("pthread_mutexattr_gettype() is not declared as int pthread_mutexattr_gettype(const pthread_mutexattr_t *restrict, int *restrict)");
	if (IS_T(&(pthread_mutexattr_gettype), int (*)(const pthread_mutexattr_t *restrict, int *restrict, int)))
		pc_fail("pthread_mutexattr_gettype() is declared without a prototype");
	if (!IS_T(&(pthread_mutexattr_init), int (*)(pthread_mutexattr_t *)))
		pc_fail("pthread_mutexattr_init() is not declared as int pthread_mutexattr_init(pthread_mutexattr_t *)");
	if (IS_T(&(pthread_mutexattr_init), int (*)(pthread_mutexattr_t *, int)))
		pc_fail("pthread_mutexattr_init() is declared without a prototype");
	if (!IS_T(&(pthread_mutexattr_setrobust), int (*)(pthread_mutexattr_t *, int)))
		pc_fail("pthread_mutexattr_setrobust() is not declared as int pthread_mutexattr_setrobust(pthread_mutexattr_t *, int)");
	if (IS_T(&(pthread_mutexattr_setrobust), int (*)(pthread_mutexattr_t *, int, int)))
		pc_fail("pthread_mutexattr_setrobust() is declared without a prototype");
	if (!IS_T(&(pthread_mutexattr_settype), int (*)(pthread_mutexattr_t *, int)))
		pc_fail("pthread_mutexattr_settype() is not declared as int pthread_mutexattr_settype(pthread_mutexattr_t *, int)");
	if (IS_T(&(pthread_mutexattr_settype), int (*)(pthread_mutexattr_t *, int, int)))
		pc_fail("pthread_mutexattr_settype() is declared without a prototype");
	if (!IS_T(&(pthread_once), int (*)(pthread_once_t *, void (*)(void))))
		pc_fail("pthread_once() is not declared as int pthread_once(pthread_once_t *, void (*)(void))");
	if (IS_T(&(pthread_once), int (*)(pthread_once_t *, void (*)(void), int)))
		pc_fail("pthread_once() is declared without a prototype");
	if (!IS_T(&(pthread_rwlock_destroy), int (*)(pthread_rwlock_t *)))
		pc_fail("pthread_rwlock_destroy() is not declared as int pthread_rwlock_destroy(pthread_rwlock_t *)");
	if (IS_T(&(pthread_rwlock_destroy), int (*)(pthread_rwlock_t *, int)))
		pc_fail("pthread_rwlock_destroy() is declared without a prototype");
	if (!IS_T(&(pthread_rwlock_init), int (*)(pthread_rwlock_t *restrict, const pthread_rwlockattr_t *restrict)))
		pc_fail("pthread_rwlock_init() is not declared as int pthread_rwlock_init(pthread_rwlock_t *restrict, const pthread_rwlockattr_t *restrict)");
	if (IS_T(&(pthread_rwlock_init), int (*)(pthread_rwlock_t *restrict, const pthread_rwlockattr_t *restrict, int)))
		pc_fail("pthread_rwlock_init() is declared without a prototype");
	if (!IS_T(&(pthread_rwlock_rdlock), int (*)(pthread_rwlock_t *)))
		pc_fail("pthread_rwlock_rdlock() is not declared as int pthread_rwlock_rdlock(pthread_rwlock_t *)");
	if (IS_T(&(pthread_rwlock_rdlock), int (*)(pthread_rwlock_t *, int)))
		pc_fail("pthread_rwlock_rdlock() is declared without a prototype");
	if (!IS_T(&(pthread_rwlock_timedrdlock), int (*)(pthread_rwlock_t *restrict, const struct timespec *restrict)))
		pc_fail("pthread_rwlock_timedrdlock() is not declared as int pthread_rwlock_timedrdlock(pthread_rwlock_t *restrict, const struct timespec *restrict)");
	if (IS_T(&(pthread_rwlock_timedrdlock), int (*)(pthread_rwlock_t *restrict, const struct timespec *restrict, int)))
		pc_fail("pthread_rwlock_timedrdlock() is declared without a prototype");
	if (!IS_T(&(pthread_rwlock_timedwrlock), int (*)(pthread_rwlock_t *restrict, const struct timespec *restrict)))
		pc_fail("pthread_rwlock_timedwrlock() is not declared as int pthread_rwlock_timedwrlock(pthread_rwlock_t *restrict, const struct timespec *restrict)");
	if (IS_T(&(pthread_rwlock_timedwrlock), int (*)(pthread_rwlock_t *restrict, const struct timespec *restrict, int)))
		pc_fail("pthread_rwlock_timedwrlock() is declared without a prototype");
	if (!IS_T(&(pthread_rwlock_tryrdlock), int (*)(pthread_rwlock_t *)))
		pc_fail("pthread_rwlock_tryrdlock() is not declared as int pthread_rwlock_tryrdlock(pthread_rwlock_t *)");
	if (IS_T(&(pthread_rwlock_tryrdlock), int (*)(pthread_rwlock_t *, int)))
		pc_fail("pthread_rwlock_tryrdlock() is declared without a prototype");
	if (!IS_T(&(pthread_rwlock_trywrlock), int (*)(pthread_rwlock_t *)))
		pc_fail("pthread_rwlock_trywrlock() is not declared as int pthread_rwlock_trywrlock(pthread_rwlock_t *)");
	if (IS_T(&(pthread_rwlock_trywrlock), int (*)(pthread_rwlock_t *, int)))
		pc_fail("pthread_rwlock_trywrlock() is declared without a prototype");
	if (!IS_T(&(pthread_rwlock_unlock), int (*)(pthread_rwlock_t *)))
		pc_fail("pthread_rwlock_unlock() is not declared as int pthread_rwlock_unlock(pthread_rwlock_t *)");
	if (IS_T(&(pthread_rwlock_unlock), int (*)(pthread_rwlock_t *, int)))
		pc_fail("pthread_rwlock_unlock() is declared without a prototype");
	if (!IS_T(&(pthread_rwlock_wrlock), int (*)(pthread_rwlock_t *)))
		pc_fail("pthread_rwlock_wrlock() is not declared as int pthread_rwlock_wrlock(pthread_rwlock_t *)");
	if (IS_T(&(pthread_rwlock_wrlock), int (*)(pthread_rwlock_t *, int)))
		pc_fail("pthread_rwlock_wrlock() is declared without a prototype");
	if (!IS_T(&(pthread_rwlockattr_destroy), int (*)(pthread_rwlockattr_t *)))
		pc_fail("pthread_rwlockattr_destroy() is not declared as int pthread_rwlockattr_destroy(pthread_rwlockattr_t *)");
	if (IS_T(&(pthread_rwlockattr_destroy), int (*)(pthread_rwlockattr_t *, int)))
		pc_fail("pthread_rwlockattr_destroy() is declared without a prototype");
	if (!IS_T(&(pthread_rwlockattr_init), int (*)(pthread_rwlockattr_t *)))
		pc_fail("pthread_rwlockattr_init() is not declared as int pthread_rwlockattr_init(pthread_rwlockattr_t *)");
	if (IS_T(&(pthread_rwlockattr_init), int (*)(pthread_rwlockattr_t *, int)))
		pc_fail("pthread_rwlockattr_init() is declared without a prototype");
	if (!IS_T(&(pthread_self), pthread_t (*)(void)))
		pc_fail("pthread_self() is not declared as pthread_t pthread_self(void)");
	if (IS_T(&(pthread_self), pthread_t (*)(int)))
		pc_fail("pthread_self() is declared without a prototype");
	if (!IS_T(&(pthread_setcancelstate), int (*)(int, int *)))
		pc_fail("pthread_setcancelstate() is not declared as int pthread_setcancelstate(int, int *)");
	if (IS_T(&(pthread_setcancelstate), int (*)(int, int *, int)))
		pc_fail("pthread_setcancelstate() is declared without a prototype");
	if (!IS_T(&(pthread_setcanceltype), int (*)(int, int *)))
		pc_fail("pthread_setcanceltype() is not declared as int pthread_setcanceltype(int, int *)");
	if (IS_T(&(pthread_setcanceltype), int (*)(int, int *, int)))
		pc_fail("pthread_setcanceltype() is declared without a prototype");
	if (!IS_T(&(pthread_setspecific), int (*)(pthread_key_t, const void *)))
		pc_fail("pthread_setspecific() is not declared as int pthread_setspecific(pthread_key_t, const void *)");
	if (IS_T(&(pthread_setspecific), int (*)(pthread_key_t, const void *, int)))
		pc_fail("pthread_setspecific() is declared without a prototype");
	if (!IS_T(&(pthread_spin_destroy), int (*)(pthread_spinlock_t *)))
		pc_fail("pthread_spin_destroy() is not declared as int pthread_spin_destroy(pthread_spinlock_t *)");
	if (IS_T(&(pthread_spin_destroy), int (*)(pthread_spinlock_t *, int)))
		pc_fail("pthread_spin_destroy() is declared without a prototype");
	if (!IS_T(&(pthread_spin_init), int (*)(pthread_spinlock_t *, int)))
		pc_fail("pthread_spin_init() is not declared as int pthread_spin_init(pthread_spinlock_t *, int)");
	if (IS_T(&(pthread_spin_init), int (*)(pthread_spinlock_t *, int, int)))
		pc_fail("pthread_spin_init() is declared without a prototype");
	if (!IS_T(&(pthread_spin_lock), int (*)(pthread_spinlock_t *)))
		pc_fail("pthread_spin_lock() is not declared as int pthread_spin_lock(pthread_spinlock_t *)");
	if (IS_T(&(pthread_spin_lock), int (*)(pthread_spinlock_t *, int)))
		pc_fail("pthread_spin_lock() is declared without a prototype");
	if (!IS_T(&(pthread_spin_trylock), int (*)(pthread_spinlock_t *)))
		pc_fail("pthread_spin_trylock() is not declared as int pthread_spin_trylock(pthread_spinlock_t *)");
	if (IS_T(&(pthread_spin_trylock), int (*)(pthread_spinlock_t *, int)))
		pc_fail("pthread_spin_trylock() is declared without a prototype");
	if (!IS_T(&(pthread_spin_unlock), int (*)(pthread_spinlock_t *)))
		pc_fail("pthread_spin_unlock() is not declared as int pthread_spin_unlock(pthread_spinlock_t *)");
	if (IS_T(&(pthread_spin_unlock), int (*)(pthread_spinlock_t *, int)))
		pc_fail("pthread_spin_unlock() is declared without a prototype");
	if (!IS_T(&(pthread_testcancel), void (*)(void)))
		pc_fail("pthread_testcancel() is not declared as void pthread_testcancel(void)");
	if (IS_T(&(pthread_testcancel), void (*)(int)))
		pc_fail("pthread_testcancel() is declared without a prototype");
	pc_pass();
}
