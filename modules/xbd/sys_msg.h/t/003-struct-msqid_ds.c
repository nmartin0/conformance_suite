/*
 * <sys/msg.h> struct msqid_ds
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sys_msg.h.003
 *
 * Only <sys/msg.h> is included (not pc.h, which brings in <stddef.h>), so a name
 * the header fails to provide stops the build, which the module reports as FAIL.
 */
#include <sys/msg.h>

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

typedef char pc_c0[sizeof(struct msqid_ds)];	/* a complete type */
static struct msqid_ds *ps;

int main(void)
{
	pc_begin("XBD.sys_msg.h.003");
	pc_test();
	(void)ps;
	if (!IS_T(&ps->msg_perm, struct ipc_perm *))
		pc_fail("struct msqid_ds member msg_perm is not of type struct ipc_perm");
	if (!IS_T(&ps->msg_qnum, msgqnum_t *))
		pc_fail("struct msqid_ds member msg_qnum is not of type msgqnum_t");
	if (!IS_T(&ps->msg_qbytes, msglen_t *))
		pc_fail("struct msqid_ds member msg_qbytes is not of type msglen_t");
	if (!IS_T(&ps->msg_lspid, pid_t *))
		pc_fail("struct msqid_ds member msg_lspid is not of type pid_t");
	if (!IS_T(&ps->msg_lrpid, pid_t *))
		pc_fail("struct msqid_ds member msg_lrpid is not of type pid_t");
	if (!IS_T(&ps->msg_stime, time_t *))
		pc_fail("struct msqid_ds member msg_stime is not of type time_t");
	if (!IS_T(&ps->msg_rtime, time_t *))
		pc_fail("struct msqid_ds member msg_rtime is not of type time_t");
	if (!IS_T(&ps->msg_ctime, time_t *))
		pc_fail("struct msqid_ds member msg_ctime is not of type time_t");
	pc_pass();
}
