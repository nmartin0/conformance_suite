/*
 * <sys/msg.h> includes ipc
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sys_msg.h.006
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

typedef char pc_c0[sizeof(struct ipc_perm)];

int main(void)
{
	pc_begin("XBD.sys_msg.h.006");
	pc_test();
#ifndef IPC_CREAT
	pc_fail("<sys/msg.h> does not define IPC_CREAT");
#endif
#ifndef IPC_EXCL
	pc_fail("<sys/msg.h> does not define IPC_EXCL");
#endif
#ifndef IPC_NOWAIT
	pc_fail("<sys/msg.h> does not define IPC_NOWAIT");
#endif
#ifndef IPC_PRIVATE
	pc_fail("<sys/msg.h> does not define IPC_PRIVATE");
#endif
#ifndef IPC_RMID
	pc_fail("<sys/msg.h> does not define IPC_RMID");
#endif
#ifndef IPC_SET
	pc_fail("<sys/msg.h> does not define IPC_SET");
#endif
#ifndef IPC_STAT
	pc_fail("<sys/msg.h> does not define IPC_STAT");
#endif
	pc_pass();
}
