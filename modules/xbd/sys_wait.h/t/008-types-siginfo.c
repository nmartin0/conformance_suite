/*
 * <sys/wait.h> types siginfo
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sys_wait.h.008
 *
 * Only <sys/wait.h> is included (not pc.h, which brings in <stddef.h>), so a name
 * the header fails to provide stops the build, which the module reports as FAIL.
 */
#include <sys/wait.h>

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

typedef siginfo_t *pc_t0;
typedef char pc_c1[sizeof(union sigval)];

int main(void)
{
	pc_begin("XBD.sys_wait.h.008");
	pc_test();

	pc_pass();
}
