/*
 * <spawn.h> functions spn
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.spawn.h.007
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
	pc_begin("XBD.spawn.h.007");
	pc_test();
	if (!IS_T(&(posix_spawn), int (*)(pid_t *restrict, const char *restrict, const posix_spawn_file_actions_t *restrict, const posix_spawnattr_t *restrict, char *const [restrict], char *const [restrict])))
		pc_fail("posix_spawn() is not declared as int posix_spawn(pid_t *restrict, const char *restrict, const posix_spawn_file_actions_t *restrict, const posix_spawnattr_t *restrict, char *const [restrict], char *const [restrict])");
	if (IS_T(&(posix_spawn), int (*)(pid_t *restrict, const char *restrict, const posix_spawn_file_actions_t *restrict, const posix_spawnattr_t *restrict, char *const [restrict], char *const [restrict], int)))
		pc_fail("posix_spawn() is declared without a prototype");
	if (!IS_T(&(posix_spawn_file_actions_addchdir), int (*)(posix_spawn_file_actions_t *restrict, const char *restrict)))
		pc_fail("posix_spawn_file_actions_addchdir() is not declared as int posix_spawn_file_actions_addchdir(posix_spawn_file_actions_t *restrict, const char *restrict)");
	if (IS_T(&(posix_spawn_file_actions_addchdir), int (*)(posix_spawn_file_actions_t *restrict, const char *restrict, int)))
		pc_fail("posix_spawn_file_actions_addchdir() is declared without a prototype");
	if (!IS_T(&(posix_spawn_file_actions_addclose), int (*)(posix_spawn_file_actions_t *, int)))
		pc_fail("posix_spawn_file_actions_addclose() is not declared as int posix_spawn_file_actions_addclose(posix_spawn_file_actions_t *, int)");
	if (IS_T(&(posix_spawn_file_actions_addclose), int (*)(posix_spawn_file_actions_t *, int, int)))
		pc_fail("posix_spawn_file_actions_addclose() is declared without a prototype");
	if (!IS_T(&(posix_spawn_file_actions_adddup2), int (*)(posix_spawn_file_actions_t *, int, int)))
		pc_fail("posix_spawn_file_actions_adddup2() is not declared as int posix_spawn_file_actions_adddup2(posix_spawn_file_actions_t *, int, int)");
	if (IS_T(&(posix_spawn_file_actions_adddup2), int (*)(posix_spawn_file_actions_t *, int, int, int)))
		pc_fail("posix_spawn_file_actions_adddup2() is declared without a prototype");
	if (!IS_T(&(posix_spawn_file_actions_addfchdir), int (*)(posix_spawn_file_actions_t *, int)))
		pc_fail("posix_spawn_file_actions_addfchdir() is not declared as int posix_spawn_file_actions_addfchdir(posix_spawn_file_actions_t *, int)");
	if (IS_T(&(posix_spawn_file_actions_addfchdir), int (*)(posix_spawn_file_actions_t *, int, int)))
		pc_fail("posix_spawn_file_actions_addfchdir() is declared without a prototype");
	if (!IS_T(&(posix_spawn_file_actions_addopen), int (*)(posix_spawn_file_actions_t *restrict, int, const char *restrict, int, mode_t)))
		pc_fail("posix_spawn_file_actions_addopen() is not declared as int posix_spawn_file_actions_addopen(posix_spawn_file_actions_t *restrict, int, const char *restrict, int, mode_t)");
	if (IS_T(&(posix_spawn_file_actions_addopen), int (*)(posix_spawn_file_actions_t *restrict, int, const char *restrict, int, mode_t, int)))
		pc_fail("posix_spawn_file_actions_addopen() is declared without a prototype");
	if (!IS_T(&(posix_spawn_file_actions_destroy), int (*)(posix_spawn_file_actions_t *)))
		pc_fail("posix_spawn_file_actions_destroy() is not declared as int posix_spawn_file_actions_destroy(posix_spawn_file_actions_t *)");
	if (IS_T(&(posix_spawn_file_actions_destroy), int (*)(posix_spawn_file_actions_t *, int)))
		pc_fail("posix_spawn_file_actions_destroy() is declared without a prototype");
	if (!IS_T(&(posix_spawn_file_actions_init), int (*)(posix_spawn_file_actions_t *)))
		pc_fail("posix_spawn_file_actions_init() is not declared as int posix_spawn_file_actions_init(posix_spawn_file_actions_t *)");
	if (IS_T(&(posix_spawn_file_actions_init), int (*)(posix_spawn_file_actions_t *, int)))
		pc_fail("posix_spawn_file_actions_init() is declared without a prototype");
	if (!IS_T(&(posix_spawnattr_destroy), int (*)(posix_spawnattr_t *)))
		pc_fail("posix_spawnattr_destroy() is not declared as int posix_spawnattr_destroy(posix_spawnattr_t *)");
	if (IS_T(&(posix_spawnattr_destroy), int (*)(posix_spawnattr_t *, int)))
		pc_fail("posix_spawnattr_destroy() is declared without a prototype");
	if (!IS_T(&(posix_spawnattr_getflags), int (*)(const posix_spawnattr_t *restrict, short *restrict)))
		pc_fail("posix_spawnattr_getflags() is not declared as int posix_spawnattr_getflags(const posix_spawnattr_t *restrict, short *restrict)");
	if (IS_T(&(posix_spawnattr_getflags), int (*)(const posix_spawnattr_t *restrict, short *restrict, int)))
		pc_fail("posix_spawnattr_getflags() is declared without a prototype");
	if (!IS_T(&(posix_spawnattr_getpgroup), int (*)(const posix_spawnattr_t *restrict, pid_t *restrict)))
		pc_fail("posix_spawnattr_getpgroup() is not declared as int posix_spawnattr_getpgroup(const posix_spawnattr_t *restrict, pid_t *restrict)");
	if (IS_T(&(posix_spawnattr_getpgroup), int (*)(const posix_spawnattr_t *restrict, pid_t *restrict, int)))
		pc_fail("posix_spawnattr_getpgroup() is declared without a prototype");
	if (!IS_T(&(posix_spawnattr_getsigdefault), int (*)(const posix_spawnattr_t *restrict, sigset_t *restrict)))
		pc_fail("posix_spawnattr_getsigdefault() is not declared as int posix_spawnattr_getsigdefault(const posix_spawnattr_t *restrict, sigset_t *restrict)");
	if (IS_T(&(posix_spawnattr_getsigdefault), int (*)(const posix_spawnattr_t *restrict, sigset_t *restrict, int)))
		pc_fail("posix_spawnattr_getsigdefault() is declared without a prototype");
	if (!IS_T(&(posix_spawnattr_getsigmask), int (*)(const posix_spawnattr_t *restrict, sigset_t *restrict)))
		pc_fail("posix_spawnattr_getsigmask() is not declared as int posix_spawnattr_getsigmask(const posix_spawnattr_t *restrict, sigset_t *restrict)");
	if (IS_T(&(posix_spawnattr_getsigmask), int (*)(const posix_spawnattr_t *restrict, sigset_t *restrict, int)))
		pc_fail("posix_spawnattr_getsigmask() is declared without a prototype");
	if (!IS_T(&(posix_spawnattr_init), int (*)(posix_spawnattr_t *)))
		pc_fail("posix_spawnattr_init() is not declared as int posix_spawnattr_init(posix_spawnattr_t *)");
	if (IS_T(&(posix_spawnattr_init), int (*)(posix_spawnattr_t *, int)))
		pc_fail("posix_spawnattr_init() is declared without a prototype");
	if (!IS_T(&(posix_spawnattr_setflags), int (*)(posix_spawnattr_t *, short)))
		pc_fail("posix_spawnattr_setflags() is not declared as int posix_spawnattr_setflags(posix_spawnattr_t *, short)");
	if (IS_T(&(posix_spawnattr_setflags), int (*)(posix_spawnattr_t *, short, int)))
		pc_fail("posix_spawnattr_setflags() is declared without a prototype");
	if (!IS_T(&(posix_spawnattr_setpgroup), int (*)(posix_spawnattr_t *, pid_t)))
		pc_fail("posix_spawnattr_setpgroup() is not declared as int posix_spawnattr_setpgroup(posix_spawnattr_t *, pid_t)");
	if (IS_T(&(posix_spawnattr_setpgroup), int (*)(posix_spawnattr_t *, pid_t, int)))
		pc_fail("posix_spawnattr_setpgroup() is declared without a prototype");
	if (!IS_T(&(posix_spawnattr_setsigdefault), int (*)(posix_spawnattr_t *restrict, const sigset_t *restrict)))
		pc_fail("posix_spawnattr_setsigdefault() is not declared as int posix_spawnattr_setsigdefault(posix_spawnattr_t *restrict, const sigset_t *restrict)");
	if (IS_T(&(posix_spawnattr_setsigdefault), int (*)(posix_spawnattr_t *restrict, const sigset_t *restrict, int)))
		pc_fail("posix_spawnattr_setsigdefault() is declared without a prototype");
	if (!IS_T(&(posix_spawnattr_setsigmask), int (*)(posix_spawnattr_t *restrict, const sigset_t *restrict)))
		pc_fail("posix_spawnattr_setsigmask() is not declared as int posix_spawnattr_setsigmask(posix_spawnattr_t *restrict, const sigset_t *restrict)");
	if (IS_T(&(posix_spawnattr_setsigmask), int (*)(posix_spawnattr_t *restrict, const sigset_t *restrict, int)))
		pc_fail("posix_spawnattr_setsigmask() is declared without a prototype");
	if (!IS_T(&(posix_spawnp), int (*)(pid_t *restrict, const char *restrict, const posix_spawn_file_actions_t *restrict, const posix_spawnattr_t *restrict, char *const [restrict], char *const [restrict])))
		pc_fail("posix_spawnp() is not declared as int posix_spawnp(pid_t *restrict, const char *restrict, const posix_spawn_file_actions_t *restrict, const posix_spawnattr_t *restrict, char *const [restrict], char *const [restrict])");
	if (IS_T(&(posix_spawnp), int (*)(pid_t *restrict, const char *restrict, const posix_spawn_file_actions_t *restrict, const posix_spawnattr_t *restrict, char *const [restrict], char *const [restrict], int)))
		pc_fail("posix_spawnp() is declared without a prototype");
	pc_pass();
}
