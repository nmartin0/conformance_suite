/*
 * <sys/ipc.h> types xsi
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sys_ipc.h.004
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

typedef uid_t *pc_t0;
typedef gid_t *pc_t1;
typedef mode_t *pc_t2;
typedef key_t *pc_t3;

int main(void)
{
	pc_begin("XBD.sys_ipc.h.004");
	pc_test();
	pc_pass();
}
