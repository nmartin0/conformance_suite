/*
 * <pthread.h> functions tps
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.pthread.h.020
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
	pc_begin("XBD.pthread.h.020");
	pc_test();
	if (!IS_T(&(pthread_attr_getinheritsched), int (*)(const pthread_attr_t *restrict, int *restrict)))
		pc_fail("pthread_attr_getinheritsched() is not declared as int pthread_attr_getinheritsched(const pthread_attr_t *restrict, int *restrict)");
	if (IS_T(&(pthread_attr_getinheritsched), int (*)(const pthread_attr_t *restrict, int *restrict, int)))
		pc_fail("pthread_attr_getinheritsched() is declared without a prototype");
	if (!IS_T(&(pthread_attr_getschedpolicy), int (*)(const pthread_attr_t *restrict, int *restrict)))
		pc_fail("pthread_attr_getschedpolicy() is not declared as int pthread_attr_getschedpolicy(const pthread_attr_t *restrict, int *restrict)");
	if (IS_T(&(pthread_attr_getschedpolicy), int (*)(const pthread_attr_t *restrict, int *restrict, int)))
		pc_fail("pthread_attr_getschedpolicy() is declared without a prototype");
	if (!IS_T(&(pthread_attr_getscope), int (*)(const pthread_attr_t *restrict, int *restrict)))
		pc_fail("pthread_attr_getscope() is not declared as int pthread_attr_getscope(const pthread_attr_t *restrict, int *restrict)");
	if (IS_T(&(pthread_attr_getscope), int (*)(const pthread_attr_t *restrict, int *restrict, int)))
		pc_fail("pthread_attr_getscope() is declared without a prototype");
	if (!IS_T(&(pthread_attr_setinheritsched), int (*)(pthread_attr_t *, int)))
		pc_fail("pthread_attr_setinheritsched() is not declared as int pthread_attr_setinheritsched(pthread_attr_t *, int)");
	if (IS_T(&(pthread_attr_setinheritsched), int (*)(pthread_attr_t *, int, int)))
		pc_fail("pthread_attr_setinheritsched() is declared without a prototype");
	if (!IS_T(&(pthread_attr_setschedpolicy), int (*)(pthread_attr_t *, int)))
		pc_fail("pthread_attr_setschedpolicy() is not declared as int pthread_attr_setschedpolicy(pthread_attr_t *, int)");
	if (IS_T(&(pthread_attr_setschedpolicy), int (*)(pthread_attr_t *, int, int)))
		pc_fail("pthread_attr_setschedpolicy() is declared without a prototype");
	if (!IS_T(&(pthread_attr_setscope), int (*)(pthread_attr_t *, int)))
		pc_fail("pthread_attr_setscope() is not declared as int pthread_attr_setscope(pthread_attr_t *, int)");
	if (IS_T(&(pthread_attr_setscope), int (*)(pthread_attr_t *, int, int)))
		pc_fail("pthread_attr_setscope() is declared without a prototype");
	if (!IS_T(&(pthread_getschedparam), int (*)(pthread_t, int *restrict, struct sched_param *restrict)))
		pc_fail("pthread_getschedparam() is not declared as int pthread_getschedparam(pthread_t, int *restrict, struct sched_param *restrict)");
	if (IS_T(&(pthread_getschedparam), int (*)(pthread_t, int *restrict, struct sched_param *restrict, int)))
		pc_fail("pthread_getschedparam() is declared without a prototype");
	if (!IS_T(&(pthread_setschedparam), int (*)(pthread_t, int, const struct sched_param *)))
		pc_fail("pthread_setschedparam() is not declared as int pthread_setschedparam(pthread_t, int, const struct sched_param *)");
	if (IS_T(&(pthread_setschedparam), int (*)(pthread_t, int, const struct sched_param *, int)))
		pc_fail("pthread_setschedparam() is declared without a prototype");
	if (!IS_T(&(pthread_setschedprio), int (*)(pthread_t, int)))
		pc_fail("pthread_setschedprio() is not declared as int pthread_setschedprio(pthread_t, int)");
	if (IS_T(&(pthread_setschedprio), int (*)(pthread_t, int, int)))
		pc_fail("pthread_setschedprio() is declared without a prototype");
	pc_pass();
}
