/*
 * <pthread.h> functions rpi rpp tpi tpp
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.pthread.h.017
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
	pc_begin("XBD.pthread.h.017");
	pc_test();
	if (!IS_T(&(pthread_mutexattr_getprotocol), int (*)(const pthread_mutexattr_t *restrict, int *restrict)))
		pc_fail("pthread_mutexattr_getprotocol() is not declared as int pthread_mutexattr_getprotocol(const pthread_mutexattr_t *restrict, int *restrict)");
	if (IS_T(&(pthread_mutexattr_getprotocol), int (*)(const pthread_mutexattr_t *restrict, int *restrict, int)))
		pc_fail("pthread_mutexattr_getprotocol() is declared without a prototype");
	if (!IS_T(&(pthread_mutexattr_setprotocol), int (*)(pthread_mutexattr_t *, int)))
		pc_fail("pthread_mutexattr_setprotocol() is not declared as int pthread_mutexattr_setprotocol(pthread_mutexattr_t *, int)");
	if (IS_T(&(pthread_mutexattr_setprotocol), int (*)(pthread_mutexattr_t *, int, int)))
		pc_fail("pthread_mutexattr_setprotocol() is declared without a prototype");
	pc_pass();
}
