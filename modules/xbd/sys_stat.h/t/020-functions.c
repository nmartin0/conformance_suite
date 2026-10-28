/*
 * <sys/stat.h> functions
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sys_stat.h.020
 *
 * Only <sys/stat.h> is included (not pc.h, which brings in <stddef.h>), so a name
 * the header fails to provide stops the build, which the module reports as FAIL.
 */
#include <sys/stat.h>

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

int main(void)
{
	pc_begin("XBD.sys_stat.h.020");
	pc_test();
	if (!IS_T(&(chmod), int (*)(const char *, mode_t)))
		pc_fail("chmod() is not declared as int chmod(const char *, mode_t)");
	if (IS_T(&(chmod), int (*)(const char *, mode_t, int)))
		pc_fail("chmod() is declared without a prototype");
	if (!IS_T(&(fchmod), int (*)(int, mode_t)))
		pc_fail("fchmod() is not declared as int fchmod(int, mode_t)");
	if (IS_T(&(fchmod), int (*)(int, mode_t, int)))
		pc_fail("fchmod() is declared without a prototype");
	if (!IS_T(&(fchmodat), int (*)(int, const char *, mode_t, int)))
		pc_fail("fchmodat() is not declared as int fchmodat(int, const char *, mode_t, int)");
	if (IS_T(&(fchmodat), int (*)(int, const char *, mode_t, int, int)))
		pc_fail("fchmodat() is declared without a prototype");
	if (!IS_T(&(fstat), int (*)(int, struct stat *)))
		pc_fail("fstat() is not declared as int fstat(int, struct stat *)");
	if (IS_T(&(fstat), int (*)(int, struct stat *, int)))
		pc_fail("fstat() is declared without a prototype");
	if (!IS_T(&(fstatat), int (*)(int, const char *restrict, struct stat *restrict, int)))
		pc_fail("fstatat() is not declared as int fstatat(int, const char *restrict, struct stat *restrict, int)");
	if (IS_T(&(fstatat), int (*)(int, const char *restrict, struct stat *restrict, int, int)))
		pc_fail("fstatat() is declared without a prototype");
	if (!IS_T(&(futimens), int (*)(int, const struct timespec [2])))
		pc_fail("futimens() is not declared as int futimens(int, const struct timespec [2])");
	if (IS_T(&(futimens), int (*)(int, const struct timespec [2], int)))
		pc_fail("futimens() is declared without a prototype");
	if (!IS_T(&(lstat), int (*)(const char *restrict, struct stat *restrict)))
		pc_fail("lstat() is not declared as int lstat(const char *restrict, struct stat *restrict)");
	if (IS_T(&(lstat), int (*)(const char *restrict, struct stat *restrict, int)))
		pc_fail("lstat() is declared without a prototype");
	if (!IS_T(&(mkdir), int (*)(const char *, mode_t)))
		pc_fail("mkdir() is not declared as int mkdir(const char *, mode_t)");
	if (IS_T(&(mkdir), int (*)(const char *, mode_t, int)))
		pc_fail("mkdir() is declared without a prototype");
	if (!IS_T(&(mkdirat), int (*)(int, const char *, mode_t)))
		pc_fail("mkdirat() is not declared as int mkdirat(int, const char *, mode_t)");
	if (IS_T(&(mkdirat), int (*)(int, const char *, mode_t, int)))
		pc_fail("mkdirat() is declared without a prototype");
	if (!IS_T(&(mkfifo), int (*)(const char *, mode_t)))
		pc_fail("mkfifo() is not declared as int mkfifo(const char *, mode_t)");
	if (IS_T(&(mkfifo), int (*)(const char *, mode_t, int)))
		pc_fail("mkfifo() is declared without a prototype");
	if (!IS_T(&(mkfifoat), int (*)(int, const char *, mode_t)))
		pc_fail("mkfifoat() is not declared as int mkfifoat(int, const char *, mode_t)");
	if (IS_T(&(mkfifoat), int (*)(int, const char *, mode_t, int)))
		pc_fail("mkfifoat() is declared without a prototype");
	if (!IS_T(&(stat), int (*)(const char *restrict, struct stat *restrict)))
		pc_fail("stat() is not declared as int stat(const char *restrict, struct stat *restrict)");
	if (IS_T(&(stat), int (*)(const char *restrict, struct stat *restrict, int)))
		pc_fail("stat() is declared without a prototype");
	if (!IS_T(&(umask), mode_t (*)(mode_t)))
		pc_fail("umask() is not declared as mode_t umask(mode_t)");
	if (IS_T(&(umask), mode_t (*)(mode_t, int)))
		pc_fail("umask() is declared without a prototype");
	if (!IS_T(&(utimensat), int (*)(int, const char *, const struct timespec [2], int)))
		pc_fail("utimensat() is not declared as int utimensat(int, const char *, const struct timespec [2], int)");
	if (IS_T(&(utimensat), int (*)(int, const char *, const struct timespec [2], int, int)))
		pc_fail("utimensat() is declared without a prototype");
	pc_pass();
}
