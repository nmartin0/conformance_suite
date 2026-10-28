/*
 * <inttypes.h> functions
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.inttypes.h.007
 *
 * Only <inttypes.h> is included (not pc.h, which brings in <stddef.h>), so a name
 * the header fails to provide stops the build, which the module reports as FAIL.
 */
#include <inttypes.h>

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

int main(void)
{
	pc_begin("XBD.inttypes.h.007");
	pc_test();
	if (!IS_T(&(imaxabs), intmax_t (*)(intmax_t)))
		pc_fail("imaxabs() is not declared as intmax_t imaxabs(intmax_t)");
	if (IS_T(&(imaxabs), intmax_t (*)(intmax_t, int)))
		pc_fail("imaxabs() is declared without a prototype");
	if (!IS_T(&(imaxdiv), imaxdiv_t (*)(intmax_t, intmax_t)))
		pc_fail("imaxdiv() is not declared as imaxdiv_t imaxdiv(intmax_t, intmax_t)");
	if (IS_T(&(imaxdiv), imaxdiv_t (*)(intmax_t, intmax_t, int)))
		pc_fail("imaxdiv() is declared without a prototype");
	if (!IS_T(&(strtoimax), intmax_t (*)(const char *restrict, char **restrict, int)))
		pc_fail("strtoimax() is not declared as intmax_t strtoimax(const char *restrict, char **restrict, int)");
	if (IS_T(&(strtoimax), intmax_t (*)(const char *restrict, char **restrict, int, int)))
		pc_fail("strtoimax() is declared without a prototype");
	if (!IS_T(&(strtoumax), uintmax_t (*)(const char *restrict, char **restrict, int)))
		pc_fail("strtoumax() is not declared as uintmax_t strtoumax(const char *restrict, char **restrict, int)");
	if (IS_T(&(strtoumax), uintmax_t (*)(const char *restrict, char **restrict, int, int)))
		pc_fail("strtoumax() is declared without a prototype");
	if (!IS_T(&(wcstoimax), intmax_t (*)(const wchar_t *restrict, wchar_t **restrict, int)))
		pc_fail("wcstoimax() is not declared as intmax_t wcstoimax(const wchar_t *restrict, wchar_t **restrict, int)");
	if (IS_T(&(wcstoimax), intmax_t (*)(const wchar_t *restrict, wchar_t **restrict, int, int)))
		pc_fail("wcstoimax() is declared without a prototype");
	if (!IS_T(&(wcstoumax), uintmax_t (*)(const wchar_t *restrict, wchar_t **restrict, int)))
		pc_fail("wcstoumax() is not declared as uintmax_t wcstoumax(const wchar_t *restrict, wchar_t **restrict, int)");
	if (IS_T(&(wcstoumax), uintmax_t (*)(const wchar_t *restrict, wchar_t **restrict, int, int)))
		pc_fail("wcstoumax() is declared without a prototype");
	pc_pass();
}
