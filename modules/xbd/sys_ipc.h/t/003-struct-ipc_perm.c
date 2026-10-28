/*
 * <sys/ipc.h> struct ipc_perm
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sys_ipc.h.003
 *
 * Only <sys/ipc.h> is included (not pc.h, which brings in <stddef.h>), so a name
 * the header fails to provide stops the build, which the module reports as FAIL.
 */
#include <sys/ipc.h>

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

typedef char pc_c0[sizeof(struct ipc_perm)];	/* a complete type */
static struct ipc_perm *ps;

int main(void)
{
	pc_begin("XBD.sys_ipc.h.003");
	pc_test();
	(void)ps;
	if (!IS_T(&ps->uid, uid_t *))
		pc_fail("struct ipc_perm member uid is not of type uid_t");
	if (!IS_T(&ps->gid, gid_t *))
		pc_fail("struct ipc_perm member gid is not of type gid_t");
	if (!IS_T(&ps->cuid, uid_t *))
		pc_fail("struct ipc_perm member cuid is not of type uid_t");
	if (!IS_T(&ps->cgid, gid_t *))
		pc_fail("struct ipc_perm member cgid is not of type gid_t");
	if (!IS_T(&ps->mode, mode_t *))
		pc_fail("struct ipc_perm member mode is not of type mode_t");
	pc_pass();
}
