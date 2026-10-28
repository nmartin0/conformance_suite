/*
 * <semaphore.h> functions sem clockwait
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.semaphore.h.005
 *
 * Only <semaphore.h> is included (not pc.h, which brings in <stddef.h>), so a name
 * the header fails to provide stops the build, which the module reports as FAIL.
 */
#include <semaphore.h>

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

int main(void)
{
	pc_begin("XBD.semaphore.h.005");
	pc_test();
	if (!IS_T(&(sem_clockwait), int (*)(sem_t *restrict, clockid_t, const struct timespec *restrict)))
		pc_fail("sem_clockwait() is not declared as int sem_clockwait(sem_t *restrict, clockid_t, const struct timespec *restrict)");
	if (IS_T(&(sem_clockwait), int (*)(sem_t *restrict, clockid_t, const struct timespec *restrict, int)))
		pc_fail("sem_clockwait() is declared without a prototype");
	pc_pass();
}
