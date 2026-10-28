/*
 * <wordexp.h> functions
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.wordexp.h.005
 *
 * Only <wordexp.h> is included (not pc.h, which brings in <stddef.h>), so a name
 * the header fails to provide stops the build, which the module reports as FAIL.
 */
#include <wordexp.h>

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

int main(void)
{
	pc_begin("XBD.wordexp.h.005");
	pc_test();
	if (!IS_T(&(wordexp), int (*)(const char *restrict, wordexp_t *restrict, int)))
		pc_fail("wordexp() is not declared as int wordexp(const char *restrict, wordexp_t *restrict, int)");
	if (IS_T(&(wordexp), int (*)(const char *restrict, wordexp_t *restrict, int, int)))
		pc_fail("wordexp() is declared without a prototype");
	if (!IS_T(&(wordfree), void (*)(wordexp_t *)))
		pc_fail("wordfree() is not declared as void wordfree(wordexp_t *)");
	if (IS_T(&(wordfree), void (*)(wordexp_t *, int)))
		pc_fail("wordfree() is declared without a prototype");
	pc_pass();
}
