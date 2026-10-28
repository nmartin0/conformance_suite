/*
 * <dirent.h> functions xsi
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.dirent.h.009
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
	pc_begin("XBD.dirent.h.009");
	pc_test();
	if (!IS_T(&(seekdir), void (*)(DIR *, long)))
		pc_fail("seekdir() is not declared as void seekdir(DIR *, long)");
	if (IS_T(&(seekdir), void (*)(DIR *, long, int)))
		pc_fail("seekdir() is declared without a prototype");
	if (!IS_T(&(telldir), long (*)(DIR *)))
		pc_fail("telldir() is not declared as long telldir(DIR *)");
	if (IS_T(&(telldir), long (*)(DIR *, int)))
		pc_fail("telldir() is declared without a prototype");
	pc_pass();
}
