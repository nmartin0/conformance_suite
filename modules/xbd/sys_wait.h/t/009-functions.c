/*
 * <sys/wait.h> functions
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sys_wait.h.009
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

int main(void)
{
	pc_begin("XBD.sys_wait.h.009");
	pc_test();
	if (!IS_T(&(wait), pid_t (*)(int *)))
		pc_fail("wait() is not declared as pid_t wait(int *)");
	if (IS_T(&(wait), pid_t (*)(int *, int)))
		pc_fail("wait() is declared without a prototype");
	if (!IS_T(&(waitid), int (*)(idtype_t, id_t, siginfo_t *, int)))
		pc_fail("waitid() is not declared as int waitid(idtype_t, id_t, siginfo_t *, int)");
	if (IS_T(&(waitid), int (*)(idtype_t, id_t, siginfo_t *, int, int)))
		pc_fail("waitid() is declared without a prototype");
	if (!IS_T(&(waitpid), pid_t (*)(pid_t, int *, int)))
		pc_fail("waitpid() is not declared as pid_t waitpid(pid_t, int *, int)");
	if (IS_T(&(waitpid), pid_t (*)(pid_t, int *, int, int)))
		pc_fail("waitpid() is declared without a prototype");
	pc_pass();
}
