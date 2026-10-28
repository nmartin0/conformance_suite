/*
 * <spawn.h> functions spn ps
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.spawn.h.006
 *
 * Only <spawn.h> is included (not pc.h, which brings in <stddef.h>), so a name
 * the header fails to provide stops the build, which the module reports as FAIL.
 */
#include <spawn.h>

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

int main(void)
{
	pc_begin("XBD.spawn.h.006");
	pc_test();
	if (!IS_T(&(posix_spawnattr_getschedparam), int (*)(const posix_spawnattr_t *restrict, struct sched_param *restrict)))
		pc_fail("posix_spawnattr_getschedparam() is not declared as int posix_spawnattr_getschedparam(const posix_spawnattr_t *restrict, struct sched_param *restrict)");
	if (IS_T(&(posix_spawnattr_getschedparam), int (*)(const posix_spawnattr_t *restrict, struct sched_param *restrict, int)))
		pc_fail("posix_spawnattr_getschedparam() is declared without a prototype");
	if (!IS_T(&(posix_spawnattr_getschedpolicy), int (*)(const posix_spawnattr_t *restrict, int *restrict)))
		pc_fail("posix_spawnattr_getschedpolicy() is not declared as int posix_spawnattr_getschedpolicy(const posix_spawnattr_t *restrict, int *restrict)");
	if (IS_T(&(posix_spawnattr_getschedpolicy), int (*)(const posix_spawnattr_t *restrict, int *restrict, int)))
		pc_fail("posix_spawnattr_getschedpolicy() is declared without a prototype");
	if (!IS_T(&(posix_spawnattr_setschedparam), int (*)(posix_spawnattr_t *restrict, const struct sched_param *restrict)))
		pc_fail("posix_spawnattr_setschedparam() is not declared as int posix_spawnattr_setschedparam(posix_spawnattr_t *restrict, const struct sched_param *restrict)");
	if (IS_T(&(posix_spawnattr_setschedparam), int (*)(posix_spawnattr_t *restrict, const struct sched_param *restrict, int)))
		pc_fail("posix_spawnattr_setschedparam() is declared without a prototype");
	if (!IS_T(&(posix_spawnattr_setschedpolicy), int (*)(posix_spawnattr_t *, int)))
		pc_fail("posix_spawnattr_setschedpolicy() is not declared as int posix_spawnattr_setschedpolicy(posix_spawnattr_t *, int)");
	if (IS_T(&(posix_spawnattr_setschedpolicy), int (*)(posix_spawnattr_t *, int, int)))
		pc_fail("posix_spawnattr_setschedpolicy() is declared without a prototype");
	pc_pass();
}
