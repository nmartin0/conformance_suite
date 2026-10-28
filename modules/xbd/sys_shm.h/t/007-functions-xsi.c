/*
 * <sys/shm.h> functions xsi
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sys_shm.h.007
 *
 * Only <sys/shm.h> is included (not pc.h, which brings in <stddef.h>), so a name
 * the header fails to provide stops the build, which the module reports as FAIL.
 */
#include <sys/shm.h>

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

int main(void)
{
	pc_begin("XBD.sys_shm.h.007");
	pc_test();
	if (!IS_T(&(shmat), void *(*)(int, const void *, int)))
		pc_fail("shmat() is not declared as void *shmat(int, const void *, int)");
	if (IS_T(&(shmat), void *(*)(int, const void *, int, int)))
		pc_fail("shmat() is declared without a prototype");
	if (!IS_T(&(shmctl), int (*)(int, int, struct shmid_ds *)))
		pc_fail("shmctl() is not declared as int shmctl(int, int, struct shmid_ds *)");
	if (IS_T(&(shmctl), int (*)(int, int, struct shmid_ds *, int)))
		pc_fail("shmctl() is declared without a prototype");
	if (!IS_T(&(shmdt), int (*)(const void *)))
		pc_fail("shmdt() is not declared as int shmdt(const void *)");
	if (IS_T(&(shmdt), int (*)(const void *, int)))
		pc_fail("shmdt() is declared without a prototype");
	if (!IS_T(&(shmget), int (*)(key_t, size_t, int)))
		pc_fail("shmget() is not declared as int shmget(key_t, size_t, int)");
	if (IS_T(&(shmget), int (*)(key_t, size_t, int, int)))
		pc_fail("shmget() is declared without a prototype");
	pc_pass();
}
