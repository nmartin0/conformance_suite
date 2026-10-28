/*
 * <sys/ipc.h> functions xsi
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sys_ipc.h.005
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

int main(void)
{
	pc_begin("XBD.sys_ipc.h.005");
	pc_test();
	if (!IS_T(&(ftok), key_t (*)(const char *, int)))
		pc_fail("ftok() is not declared as key_t ftok(const char *, int)");
	if (IS_T(&(ftok), key_t (*)(const char *, int, int)))
		pc_fail("ftok() is declared without a prototype");
	pc_pass();
}
