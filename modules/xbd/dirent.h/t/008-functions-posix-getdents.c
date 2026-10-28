/*
 * <dirent.h> functions posix getdents
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.dirent.h.008
 *
 * Only <dirent.h> is included (not pc.h, which brings in <stddef.h>), so a name
 * the header fails to provide stops the build, which the module reports as FAIL.
 */
#include <dirent.h>

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

int main(void)
{
	pc_begin("XBD.dirent.h.008");
	pc_test();
	if (!IS_T(&(posix_getdents), ssize_t (*)(int, void *, size_t, int)))
		pc_fail("posix_getdents() is not declared as ssize_t posix_getdents(int, void *, size_t, int)");
	if (IS_T(&(posix_getdents), ssize_t (*)(int, void *, size_t, int, int)))
		pc_fail("posix_getdents() is declared without a prototype");
	pc_pass();
}
