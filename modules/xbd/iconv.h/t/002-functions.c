/*
 * <iconv.h> functions
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.iconv.h.002
 *
 * Only <iconv.h> is included (not pc.h, which brings in <stddef.h>), so a name
 * the header fails to provide stops the build, which the module reports as FAIL.
 */
#include <iconv.h>

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

int main(void)
{
	pc_begin("XBD.iconv.h.002");
	pc_test();
	if (!IS_T(&(iconv), size_t (*)(iconv_t, char **, size_t *, char **, size_t *)))
		pc_fail("iconv() is not declared as size_t iconv(iconv_t, char **restrict, size_t *restrict, char **restrict, size_t *restrict)");
	if (IS_T(&(iconv), size_t (*)(iconv_t, char **, size_t *, char **, size_t *, int)))
		pc_fail("iconv() is declared without a prototype");
	if (!IS_T(&(iconv_close), int (*)(iconv_t)))
		pc_fail("iconv_close() is not declared as int iconv_close(iconv_t)");
	if (IS_T(&(iconv_close), int (*)(iconv_t, int)))
		pc_fail("iconv_close() is declared without a prototype");
	if (!IS_T(&(iconv_open), iconv_t (*)(const char *, const char *)))
		pc_fail("iconv_open() is not declared as iconv_t iconv_open(const char *, const char *)");
	if (IS_T(&(iconv_open), iconv_t (*)(const char *, const char *, int)))
		pc_fail("iconv_open() is declared without a prototype");
	pc_pass();
}
