/*
 * <regex.h> functions
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.regex.h.009
 *
 * Only <regex.h> is included (not pc.h, which brings in <stddef.h>), so a name
 * the header fails to provide stops the build, which the module reports as FAIL.
 */
#include <regex.h>

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

int main(void)
{
	pc_begin("XBD.regex.h.009");
	pc_test();
	if (!IS_T(&(regcomp), int (*)(regex_t *restrict, const char *restrict, int)))
		pc_fail("regcomp() is not declared as int regcomp(regex_t *restrict, const char *restrict, int)");
	if (IS_T(&(regcomp), int (*)(regex_t *restrict, const char *restrict, int, int)))
		pc_fail("regcomp() is declared without a prototype");
	if (!IS_T(&(regerror), size_t (*)(int, const regex_t *restrict, char *restrict, size_t)))
		pc_fail("regerror() is not declared as size_t regerror(int, const regex_t *restrict, char *restrict, size_t)");
	if (IS_T(&(regerror), size_t (*)(int, const regex_t *restrict, char *restrict, size_t, int)))
		pc_fail("regerror() is declared without a prototype");
	if (!IS_T(&(regexec), int (*)(const regex_t *restrict, const char *restrict, size_t, regmatch_t [restrict], int)))
		pc_fail("regexec() is not declared as int regexec(const regex_t *restrict, const char *restrict, size_t, regmatch_t [restrict], int)");
	if (IS_T(&(regexec), int (*)(const regex_t *restrict, const char *restrict, size_t, regmatch_t [restrict], int, int)))
		pc_fail("regexec() is declared without a prototype");
	if (!IS_T(&(regfree), void (*)(regex_t *)))
		pc_fail("regfree() is not declared as void regfree(regex_t *)");
	if (IS_T(&(regfree), void (*)(regex_t *, int)))
		pc_fail("regfree() is declared without a prototype");
	pc_pass();
}
