/*
 * <sys/statvfs.h> struct statvfs
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sys_statvfs.h.002
 *
 * Only <sys/statvfs.h> is included (not pc.h, which brings in <stddef.h>), so a name
 * the header fails to provide stops the build, which the module reports as FAIL.
 */
#include <sys/statvfs.h>

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

typedef char pc_c0[sizeof(struct statvfs)];	/* a complete type */
static struct statvfs *ps;

int main(void)
{
	pc_begin("XBD.sys_statvfs.h.002");
	pc_test();
	(void)ps;
	if (!IS_T(&ps->f_bsize, unsigned long *))
		pc_fail("struct statvfs member f_bsize is not of type unsigned long");
	if (!IS_T(&ps->f_frsize, unsigned long *))
		pc_fail("struct statvfs member f_frsize is not of type unsigned long");
	if (!IS_T(&ps->f_blocks, fsblkcnt_t *))
		pc_fail("struct statvfs member f_blocks is not of type fsblkcnt_t");
	if (!IS_T(&ps->f_bfree, fsblkcnt_t *))
		pc_fail("struct statvfs member f_bfree is not of type fsblkcnt_t");
	if (!IS_T(&ps->f_bavail, fsblkcnt_t *))
		pc_fail("struct statvfs member f_bavail is not of type fsblkcnt_t");
	if (!IS_T(&ps->f_files, fsfilcnt_t *))
		pc_fail("struct statvfs member f_files is not of type fsfilcnt_t");
	if (!IS_T(&ps->f_ffree, fsfilcnt_t *))
		pc_fail("struct statvfs member f_ffree is not of type fsfilcnt_t");
	if (!IS_T(&ps->f_favail, fsfilcnt_t *))
		pc_fail("struct statvfs member f_favail is not of type fsfilcnt_t");
	if (!IS_T(&ps->f_fsid, unsigned long *))
		pc_fail("struct statvfs member f_fsid is not of type unsigned long");
	if (!IS_T(&ps->f_flag, unsigned long *))
		pc_fail("struct statvfs member f_flag is not of type unsigned long");
	if (!IS_T(&ps->f_namemax, unsigned long *))
		pc_fail("struct statvfs member f_namemax is not of type unsigned long");
	pc_pass();
}
