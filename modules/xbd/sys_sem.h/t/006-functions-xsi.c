/*
 * <sys/sem.h> functions xsi
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sys_sem.h.006
 *
 * Only <sys/sem.h> is included (not pc.h, which brings in <stddef.h>), so a name
 * the header fails to provide stops the build, which the module reports as FAIL.
 */
#include <sys/sem.h>

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

int main(void)
{
	pc_begin("XBD.sys_sem.h.006");
	pc_test();
	if (!IS_T(&(semctl), int (*)(int, int, int, ...)))
		pc_fail("semctl() is not declared as int semctl(int, int, int, ...)");
	if (!IS_T(&(semget), int (*)(key_t, int, int)))
		pc_fail("semget() is not declared as int semget(key_t, int, int)");
	if (IS_T(&(semget), int (*)(key_t, int, int, int)))
		pc_fail("semget() is declared without a prototype");
	if (!IS_T(&(semop), int (*)(int, struct sembuf *, size_t)))
		pc_fail("semop() is not declared as int semop(int, struct sembuf *, size_t)");
	if (IS_T(&(semop), int (*)(int, struct sembuf *, size_t, int)))
		pc_fail("semop() is declared without a prototype");
	pc_pass();
}
