/*
 * <semaphore.h> functions
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.semaphore.h.006
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
	pc_begin("XBD.semaphore.h.006");
	pc_test();
	if (!IS_T(&(sem_close), int (*)(sem_t *)))
		pc_fail("sem_close() is not declared as int sem_close(sem_t *)");
	if (IS_T(&(sem_close), int (*)(sem_t *, int)))
		pc_fail("sem_close() is declared without a prototype");
	if (!IS_T(&(sem_destroy), int (*)(sem_t *)))
		pc_fail("sem_destroy() is not declared as int sem_destroy(sem_t *)");
	if (IS_T(&(sem_destroy), int (*)(sem_t *, int)))
		pc_fail("sem_destroy() is declared without a prototype");
	if (!IS_T(&(sem_getvalue), int (*)(sem_t *restrict, int *restrict)))
		pc_fail("sem_getvalue() is not declared as int sem_getvalue(sem_t *restrict, int *restrict)");
	if (IS_T(&(sem_getvalue), int (*)(sem_t *restrict, int *restrict, int)))
		pc_fail("sem_getvalue() is declared without a prototype");
	if (!IS_T(&(sem_init), int (*)(sem_t *, int, unsigned)))
		pc_fail("sem_init() is not declared as int sem_init(sem_t *, int, unsigned)");
	if (IS_T(&(sem_init), int (*)(sem_t *, int, unsigned, int)))
		pc_fail("sem_init() is declared without a prototype");
	if (!IS_T(&(sem_open), sem_t *(*)(const char *, int, ...)))
		pc_fail("sem_open() is not declared as sem_t *sem_open(const char *, int, ...)");
	if (!IS_T(&(sem_post), int (*)(sem_t *)))
		pc_fail("sem_post() is not declared as int sem_post(sem_t *)");
	if (IS_T(&(sem_post), int (*)(sem_t *, int)))
		pc_fail("sem_post() is declared without a prototype");
	if (!IS_T(&(sem_timedwait), int (*)(sem_t *restrict, const struct timespec *restrict)))
		pc_fail("sem_timedwait() is not declared as int sem_timedwait(sem_t *restrict, const struct timespec *restrict)");
	if (IS_T(&(sem_timedwait), int (*)(sem_t *restrict, const struct timespec *restrict, int)))
		pc_fail("sem_timedwait() is declared without a prototype");
	if (!IS_T(&(sem_trywait), int (*)(sem_t *)))
		pc_fail("sem_trywait() is not declared as int sem_trywait(sem_t *)");
	if (IS_T(&(sem_trywait), int (*)(sem_t *, int)))
		pc_fail("sem_trywait() is declared without a prototype");
	if (!IS_T(&(sem_unlink), int (*)(const char *)))
		pc_fail("sem_unlink() is not declared as int sem_unlink(const char *)");
	if (IS_T(&(sem_unlink), int (*)(const char *, int)))
		pc_fail("sem_unlink() is declared without a prototype");
	if (!IS_T(&(sem_wait), int (*)(sem_t *)))
		pc_fail("sem_wait() is not declared as int sem_wait(sem_t *)");
	if (IS_T(&(sem_wait), int (*)(sem_t *, int)))
		pc_fail("sem_wait() is declared without a prototype");
	pc_pass();
}
