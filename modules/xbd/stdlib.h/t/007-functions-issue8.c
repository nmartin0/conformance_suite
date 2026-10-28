/*
 * <stdlib.h> functions issue8
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.stdlib.h.007
 *
 * Only <stdlib.h> is included (not pc.h, which brings in <stddef.h>), so a name
 * the header fails to provide stops the build, which the module reports as FAIL.
 */
#include <stdlib.h>

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

int main(void)
{
	pc_begin("XBD.stdlib.h.007");
	pc_test();
	if (!IS_T(&(mkostemp), int (*)(char *, int)))
		pc_fail("mkostemp() is not declared as int mkostemp(char *, int)");
	if (IS_T(&(mkostemp), int (*)(char *, int, int)))
		pc_fail("mkostemp() is declared without a prototype");
	if (!IS_T(&(qsort_r), void (*)(void *, size_t, size_t, int (*)(const void *, const void *, void *), void *)))
		pc_fail("qsort_r() is not declared as void qsort_r(void *, size_t, size_t, int (*)(const void *, const void *, void *), void *)");
	if (IS_T(&(qsort_r), void (*)(void *, size_t, size_t, int (*)(const void *, const void *, void *), void *, int)))
		pc_fail("qsort_r() is declared without a prototype");
	if (!IS_T(&(secure_getenv), char *(*)(const char *)))
		pc_fail("secure_getenv() is not declared as char *secure_getenv(const char *)");
	if (IS_T(&(secure_getenv), char *(*)(const char *, int)))
		pc_fail("secure_getenv() is declared without a prototype");
	if (!IS_T(&(reallocarray), void *(*)(void *, size_t, size_t)))
		pc_fail("reallocarray() is not declared as void *reallocarray(void *, size_t, size_t)");
	if (IS_T(&(reallocarray), void *(*)(void *, size_t, size_t, int)))
		pc_fail("reallocarray() is declared without a prototype");
	pc_pass();
}
