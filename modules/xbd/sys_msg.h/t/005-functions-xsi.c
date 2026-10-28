/*
 * <sys/msg.h> functions xsi
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sys_msg.h.005
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

int main(void)
{
	pc_begin("XBD.sys_msg.h.005");
	pc_test();
	if (!IS_T(&(msgctl), int (*)(int, int, struct msqid_ds *)))
		pc_fail("msgctl() is not declared as int msgctl(int, int, struct msqid_ds *)");
	if (IS_T(&(msgctl), int (*)(int, int, struct msqid_ds *, int)))
		pc_fail("msgctl() is declared without a prototype");
	if (!IS_T(&(msgget), int (*)(key_t, int)))
		pc_fail("msgget() is not declared as int msgget(key_t, int)");
	if (IS_T(&(msgget), int (*)(key_t, int, int)))
		pc_fail("msgget() is declared without a prototype");
	if (!IS_T(&(msgrcv), ssize_t (*)(int, void *, size_t, long, int)))
		pc_fail("msgrcv() is not declared as ssize_t msgrcv(int, void *, size_t, long, int)");
	if (IS_T(&(msgrcv), ssize_t (*)(int, void *, size_t, long, int, int)))
		pc_fail("msgrcv() is declared without a prototype");
	if (!IS_T(&(msgsnd), int (*)(int, const void *, size_t, int)))
		pc_fail("msgsnd() is not declared as int msgsnd(int, const void *, size_t, int)");
	if (IS_T(&(msgsnd), int (*)(int, const void *, size_t, int, int)))
		pc_fail("msgsnd() is declared without a prototype");
	pc_pass();
}
