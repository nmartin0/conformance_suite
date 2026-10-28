/*
 * <ftw.h> functions xsi
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.ftw.h.007
 *
 * Only <ftw.h> is included (not pc.h, which brings in <stddef.h>), so a name
 * the header fails to provide stops the build, which the module reports as FAIL.
 */
#include <ftw.h>

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

int main(void)
{
	pc_begin("XBD.ftw.h.007");
	pc_test();
	if (!IS_T(&(nftw), int (*)(const char *, int (*)(const char *, const struct stat *, int, struct FTW *), int, int)))
		pc_fail("nftw() is not declared as int nftw(const char *, int (*)(const char *, const struct stat *, int, struct FTW *), int, int)");
	if (IS_T(&(nftw), int (*)(const char *, int (*)(const char *, const struct stat *, int, struct FTW *), int, int, int)))
		pc_fail("nftw() is declared without a prototype");
	pc_pass();
}
