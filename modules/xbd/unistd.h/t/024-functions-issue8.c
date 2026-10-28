/*
 * <unistd.h> functions issue8
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.unistd.h.024
 *
 * Only <unistd.h> is included (not pc.h, which brings in <stddef.h>), so a name
 * the header fails to provide stops the build, which the module reports as FAIL.
 */
#include <unistd.h>

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

int main(void)
{
	pc_begin("XBD.unistd.h.024");
	pc_test();
	if (!IS_T(&(dup3), int (*)(int, int, int)))
		pc_fail("dup3() is not declared as int dup3(int, int, int)");
	if (IS_T(&(dup3), int (*)(int, int, int, int)))
		pc_fail("dup3() is declared without a prototype");
	if (!IS_T(&(_Fork), pid_t (*)(void)))
		pc_fail("_Fork() is not declared as pid_t _Fork(void)");
	if (IS_T(&(_Fork), pid_t (*)(int)))
		pc_fail("_Fork() is declared without a prototype");
	if (!IS_T(&(getentropy), int (*)(void *, size_t)))
		pc_fail("getentropy() is not declared as int getentropy(void *, size_t)");
	if (IS_T(&(getentropy), int (*)(void *, size_t, int)))
		pc_fail("getentropy() is declared without a prototype");
	if (!IS_T(&(pipe2), int (*)(int [2], int)))
		pc_fail("pipe2() is not declared as int pipe2(int [2], int)");
	if (IS_T(&(pipe2), int (*)(int [2], int, int)))
		pc_fail("pipe2() is declared without a prototype");
	if (!IS_T(&(posix_close), int (*)(int, int)))
		pc_fail("posix_close() is not declared as int posix_close(int, int)");
	if (IS_T(&(posix_close), int (*)(int, int, int)))
		pc_fail("posix_close() is declared without a prototype");
	pc_pass();
}
