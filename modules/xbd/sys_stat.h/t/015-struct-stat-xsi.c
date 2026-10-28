/*
 * <sys/stat.h> struct stat xsi
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sys_stat.h.015
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
	pc_begin("XBD.sys_stat.h.015");
	pc_test();
	(void)ps;
	if (!IS_T(&ps->st_rdev, dev_t *))
		pc_fail("struct stat member st_rdev is not of type dev_t");
	if (!IS_T(&ps->st_blksize, blksize_t *))
		pc_fail("struct stat member st_blksize is not of type blksize_t");
	if (!IS_T(&ps->st_blocks, blkcnt_t *))
		pc_fail("struct stat member st_blocks is not of type blkcnt_t");
	pc_pass();
}
