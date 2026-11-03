/*
 * <ctype.h> functions
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.ctype.h.002
 *
 * Only <ctype.h> is included (not pc.h, which brings in <stddef.h>), so a name
 * the header fails to provide stops the build, which the module reports as FAIL.
 */
#include <ctype.h>

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

int main(void)
{
	pc_begin("XBD.ctype.h.002");
	pc_test();
	if (!IS_T(&(isalnum), int (*)(int)))
		pc_fail("isalnum() is not declared as int isalnum(int)");
	if (IS_T(&(isalnum), int (*)(int, int)))
		pc_fail("isalnum() is declared without a prototype");
	if (!IS_T(&(isalpha), int (*)(int)))
		pc_fail("isalpha() is not declared as int isalpha(int)");
	if (IS_T(&(isalpha), int (*)(int, int)))
		pc_fail("isalpha() is declared without a prototype");
	if (!IS_T(&(isblank), int (*)(int)))
		pc_fail("isblank() is not declared as int isblank(int)");
	if (IS_T(&(isblank), int (*)(int, int)))
		pc_fail("isblank() is declared without a prototype");
	if (!IS_T(&(iscntrl), int (*)(int)))
		pc_fail("iscntrl() is not declared as int iscntrl(int)");
	if (IS_T(&(iscntrl), int (*)(int, int)))
		pc_fail("iscntrl() is declared without a prototype");
	if (!IS_T(&(isdigit), int (*)(int)))
		pc_fail("isdigit() is not declared as int isdigit(int)");
	if (IS_T(&(isdigit), int (*)(int, int)))
		pc_fail("isdigit() is declared without a prototype");
	if (!IS_T(&(isgraph), int (*)(int)))
		pc_fail("isgraph() is not declared as int isgraph(int)");
	if (IS_T(&(isgraph), int (*)(int, int)))
		pc_fail("isgraph() is declared without a prototype");
	if (!IS_T(&(islower), int (*)(int)))
		pc_fail("islower() is not declared as int islower(int)");
	if (IS_T(&(islower), int (*)(int, int)))
		pc_fail("islower() is declared without a prototype");
	if (!IS_T(&(isprint), int (*)(int)))
		pc_fail("isprint() is not declared as int isprint(int)");
	if (IS_T(&(isprint), int (*)(int, int)))
		pc_fail("isprint() is declared without a prototype");
	if (!IS_T(&(ispunct), int (*)(int)))
		pc_fail("ispunct() is not declared as int ispunct(int)");
	if (IS_T(&(ispunct), int (*)(int, int)))
		pc_fail("ispunct() is declared without a prototype");
	if (!IS_T(&(isspace), int (*)(int)))
		pc_fail("isspace() is not declared as int isspace(int)");
	if (IS_T(&(isspace), int (*)(int, int)))
		pc_fail("isspace() is declared without a prototype");
	if (!IS_T(&(isupper), int (*)(int)))
		pc_fail("isupper() is not declared as int isupper(int)");
	if (IS_T(&(isupper), int (*)(int, int)))
		pc_fail("isupper() is declared without a prototype");
	if (!IS_T(&(isxdigit), int (*)(int)))
		pc_fail("isxdigit() is not declared as int isxdigit(int)");
	if (IS_T(&(isxdigit), int (*)(int, int)))
		pc_fail("isxdigit() is declared without a prototype");
	if (!IS_T(&(tolower), int (*)(int)))
		pc_fail("tolower() is not declared as int tolower(int)");
	if (IS_T(&(tolower), int (*)(int, int)))
		pc_fail("tolower() is declared without a prototype");
	if (!IS_T(&(toupper), int (*)(int)))
		pc_fail("toupper() is not declared as int toupper(int)");
	if (IS_T(&(toupper), int (*)(int, int)))
		pc_fail("toupper() is declared without a prototype");
	if (!IS_T(&(isalnum_l), int (*)(int, locale_t)))
		pc_fail("isalnum_l() is not declared as int isalnum_l(int, locale_t)");
	if (IS_T(&(isalnum_l), int (*)(int, locale_t, int)))
		pc_fail("isalnum_l() is declared without a prototype");
	if (!IS_T(&(isalpha_l), int (*)(int, locale_t)))
		pc_fail("isalpha_l() is not declared as int isalpha_l(int, locale_t)");
	if (IS_T(&(isalpha_l), int (*)(int, locale_t, int)))
		pc_fail("isalpha_l() is declared without a prototype");
	if (!IS_T(&(isblank_l), int (*)(int, locale_t)))
		pc_fail("isblank_l() is not declared as int isblank_l(int, locale_t)");
	if (IS_T(&(isblank_l), int (*)(int, locale_t, int)))
		pc_fail("isblank_l() is declared without a prototype");
	if (!IS_T(&(iscntrl_l), int (*)(int, locale_t)))
		pc_fail("iscntrl_l() is not declared as int iscntrl_l(int, locale_t)");
	if (IS_T(&(iscntrl_l), int (*)(int, locale_t, int)))
		pc_fail("iscntrl_l() is declared without a prototype");
	if (!IS_T(&(isdigit_l), int (*)(int, locale_t)))
		pc_fail("isdigit_l() is not declared as int isdigit_l(int, locale_t)");
	if (IS_T(&(isdigit_l), int (*)(int, locale_t, int)))
		pc_fail("isdigit_l() is declared without a prototype");
	if (!IS_T(&(isgraph_l), int (*)(int, locale_t)))
		pc_fail("isgraph_l() is not declared as int isgraph_l(int, locale_t)");
	if (IS_T(&(isgraph_l), int (*)(int, locale_t, int)))
		pc_fail("isgraph_l() is declared without a prototype");
	if (!IS_T(&(islower_l), int (*)(int, locale_t)))
		pc_fail("islower_l() is not declared as int islower_l(int, locale_t)");
	if (IS_T(&(islower_l), int (*)(int, locale_t, int)))
		pc_fail("islower_l() is declared without a prototype");
	if (!IS_T(&(isprint_l), int (*)(int, locale_t)))
		pc_fail("isprint_l() is not declared as int isprint_l(int, locale_t)");
	if (IS_T(&(isprint_l), int (*)(int, locale_t, int)))
		pc_fail("isprint_l() is declared without a prototype");
	if (!IS_T(&(ispunct_l), int (*)(int, locale_t)))
		pc_fail("ispunct_l() is not declared as int ispunct_l(int, locale_t)");
	if (IS_T(&(ispunct_l), int (*)(int, locale_t, int)))
		pc_fail("ispunct_l() is declared without a prototype");
	if (!IS_T(&(isspace_l), int (*)(int, locale_t)))
		pc_fail("isspace_l() is not declared as int isspace_l(int, locale_t)");
	if (IS_T(&(isspace_l), int (*)(int, locale_t, int)))
		pc_fail("isspace_l() is declared without a prototype");
	if (!IS_T(&(isupper_l), int (*)(int, locale_t)))
		pc_fail("isupper_l() is not declared as int isupper_l(int, locale_t)");
	if (IS_T(&(isupper_l), int (*)(int, locale_t, int)))
		pc_fail("isupper_l() is declared without a prototype");
	if (!IS_T(&(isxdigit_l), int (*)(int, locale_t)))
		pc_fail("isxdigit_l() is not declared as int isxdigit_l(int, locale_t)");
	if (IS_T(&(isxdigit_l), int (*)(int, locale_t, int)))
		pc_fail("isxdigit_l() is declared without a prototype");
	if (!IS_T(&(tolower_l), int (*)(int, locale_t)))
		pc_fail("tolower_l() is not declared as int tolower_l(int, locale_t)");
	if (IS_T(&(tolower_l), int (*)(int, locale_t, int)))
		pc_fail("tolower_l() is declared without a prototype");
	if (!IS_T(&(toupper_l), int (*)(int, locale_t)))
		pc_fail("toupper_l() is not declared as int toupper_l(int, locale_t)");
	if (IS_T(&(toupper_l), int (*)(int, locale_t, int)))
		pc_fail("toupper_l() is declared without a prototype");
	pc_pass();
}
