/*
 * <pthread.h> functions tss
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.pthread.h.023
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
	pc_begin("XBD.pthread.h.023");
	pc_test();
	if (!IS_T(&(pthread_attr_getstacksize), int (*)(const pthread_attr_t *restrict, size_t *restrict)))
		pc_fail("pthread_attr_getstacksize() is not declared as int pthread_attr_getstacksize(const pthread_attr_t *restrict, size_t *restrict)");
	if (IS_T(&(pthread_attr_getstacksize), int (*)(const pthread_attr_t *restrict, size_t *restrict, int)))
		pc_fail("pthread_attr_getstacksize() is declared without a prototype");
	if (!IS_T(&(pthread_attr_setstacksize), int (*)(pthread_attr_t *, size_t)))
		pc_fail("pthread_attr_setstacksize() is not declared as int pthread_attr_setstacksize(pthread_attr_t *, size_t)");
	if (IS_T(&(pthread_attr_setstacksize), int (*)(pthread_attr_t *, size_t, int)))
		pc_fail("pthread_attr_setstacksize() is declared without a prototype");
	pc_pass();
}
