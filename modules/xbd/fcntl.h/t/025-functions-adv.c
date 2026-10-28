/*
 * <fcntl.h> functions adv
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.fcntl.h.025
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

int main(void)
{
	pc_begin("XBD.fcntl.h.025");
	pc_test();
	if (!IS_T(&(posix_fadvise), int (*)(int, off_t, off_t, int)))
		pc_fail("posix_fadvise() is not declared as int posix_fadvise(int, off_t, off_t, int)");
	if (IS_T(&(posix_fadvise), int (*)(int, off_t, off_t, int, int)))
		pc_fail("posix_fadvise() is declared without a prototype");
	if (!IS_T(&(posix_fallocate), int (*)(int, off_t, off_t)))
		pc_fail("posix_fallocate() is not declared as int posix_fallocate(int, off_t, off_t)");
	if (IS_T(&(posix_fallocate), int (*)(int, off_t, off_t, int)))
		pc_fail("posix_fallocate() is declared without a prototype");
	pc_pass();
}
