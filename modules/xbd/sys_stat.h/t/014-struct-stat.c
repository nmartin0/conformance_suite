/*
 * <sys/stat.h> struct stat
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sys_stat.h.014
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

typedef char pc_c0[sizeof(struct stat)];	/* a complete type */
static struct stat *ps;

int main(void)
{
	pc_begin("XBD.sys_stat.h.014");
	pc_test();
	(void)ps;
	if (!IS_T(&ps->st_dev, dev_t *))
		pc_fail("struct stat member st_dev is not of type dev_t");
	if (!IS_T(&ps->st_ino, ino_t *))
		pc_fail("struct stat member st_ino is not of type ino_t");
	if (!IS_T(&ps->st_mode, mode_t *))
		pc_fail("struct stat member st_mode is not of type mode_t");
	if (!IS_T(&ps->st_uid, uid_t *))
		pc_fail("struct stat member st_uid is not of type uid_t");
	if (!IS_T(&ps->st_gid, gid_t *))
		pc_fail("struct stat member st_gid is not of type gid_t");
	if (!IS_T(&ps->st_size, off_t *))
		pc_fail("struct stat member st_size is not of type off_t");
	if (!IS_T(&ps->st_atim, struct timespec *))
		pc_fail("struct stat member st_atim is not of type struct timespec");
	if (!IS_T(&ps->st_mtim, struct timespec *))
		pc_fail("struct stat member st_mtim is not of type struct timespec");
	if (!IS_T(&ps->st_ctim, struct timespec *))
		pc_fail("struct stat member st_ctim is not of type struct timespec");
	pc_pass();
}
