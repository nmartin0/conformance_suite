/*
 * <fcntl.h> struct flock
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.fcntl.h.017
 *
 * Only <fcntl.h> is included (not pc.h, which brings in <stddef.h>), so a name
 * the header fails to provide stops the build, which the module reports as FAIL.
 */
#include <fcntl.h>

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

typedef char pc_c0[sizeof(struct flock)];	/* a complete type */
static struct flock *ps;

int main(void)
{
	pc_begin("XBD.fcntl.h.017");
	pc_test();
	(void)ps;
	if (!IS_T(&ps->l_type, short *))
		pc_fail("struct flock member l_type is not of type short");
	if (!IS_T(&ps->l_whence, short *))
		pc_fail("struct flock member l_whence is not of type short");
	if (!IS_T(&ps->l_start, off_t *))
		pc_fail("struct flock member l_start is not of type off_t");
	if (!IS_T(&ps->l_len, off_t *))
		pc_fail("struct flock member l_len is not of type off_t");
	if (!IS_T(&ps->l_pid, pid_t *))
		pc_fail("struct flock member l_pid is not of type pid_t");
	pc_pass();
}
