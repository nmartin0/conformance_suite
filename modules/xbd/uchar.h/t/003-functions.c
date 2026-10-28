/*
 * <uchar.h> functions
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.uchar.h.003
 *
 * Only <uchar.h> is included (not pc.h, which brings in <stddef.h>), so a name
 * the header fails to provide stops the build, which the module reports as FAIL.
 */
#include <uchar.h>

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

int main(void)
{
	pc_begin("XBD.uchar.h.003");
	pc_test();
	if (!IS_T(&(c16rtomb), size_t (*)(char *restrict, char16_t, mbstate_t *restrict)))
		pc_fail("c16rtomb() is not declared as size_t c16rtomb(char *restrict, char16_t, mbstate_t *restrict)");
	if (IS_T(&(c16rtomb), size_t (*)(char *restrict, char16_t, mbstate_t *restrict, int)))
		pc_fail("c16rtomb() is declared without a prototype");
	if (!IS_T(&(c32rtomb), size_t (*)(char *restrict, char32_t, mbstate_t *restrict)))
		pc_fail("c32rtomb() is not declared as size_t c32rtomb(char *restrict, char32_t, mbstate_t *restrict)");
	if (IS_T(&(c32rtomb), size_t (*)(char *restrict, char32_t, mbstate_t *restrict, int)))
		pc_fail("c32rtomb() is declared without a prototype");
	if (!IS_T(&(mbrtoc16), size_t (*)(char16_t *restrict, const char *restrict, size_t, mbstate_t *restrict)))
		pc_fail("mbrtoc16() is not declared as size_t mbrtoc16(char16_t *restrict, const char *restrict, size_t, mbstate_t *restrict)");
	if (IS_T(&(mbrtoc16), size_t (*)(char16_t *restrict, const char *restrict, size_t, mbstate_t *restrict, int)))
		pc_fail("mbrtoc16() is declared without a prototype");
	if (!IS_T(&(mbrtoc32), size_t (*)(char32_t *restrict, const char *restrict, size_t, mbstate_t *restrict)))
		pc_fail("mbrtoc32() is not declared as size_t mbrtoc32(char32_t *restrict, const char *restrict, size_t, mbstate_t *restrict)");
	if (IS_T(&(mbrtoc32), size_t (*)(char32_t *restrict, const char *restrict, size_t, mbstate_t *restrict, int)))
		pc_fail("mbrtoc32() is declared without a prototype");
	pc_pass();
}
