/*
 * <libintl.h> functions
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.libintl.h.003
 *
 * Only <libintl.h> is included (not pc.h, which brings in <stddef.h>), so a name
 * the header fails to provide stops the build, which the module reports as FAIL.
 */
#include <libintl.h>

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

int main(void)
{
	pc_begin("XBD.libintl.h.003");
	pc_test();
	if (!IS_T(&(bindtextdomain), char *(*)(const char *, const char *)))
		pc_fail("bindtextdomain() is not declared as char *bindtextdomain(const char *, const char *)");
	if (IS_T(&(bindtextdomain), char *(*)(const char *, const char *, int)))
		pc_fail("bindtextdomain() is declared without a prototype");
	if (!IS_T(&(bind_textdomain_codeset), char *(*)(const char *, const char *)))
		pc_fail("bind_textdomain_codeset() is not declared as char *bind_textdomain_codeset(const char *, const char *)");
	if (IS_T(&(bind_textdomain_codeset), char *(*)(const char *, const char *, int)))
		pc_fail("bind_textdomain_codeset() is declared without a prototype");
	if (!IS_T(&(dcgettext), char *(*)(const char *, const char *, int)))
		pc_fail("dcgettext() is not declared as char *dcgettext(const char *, const char *, int)");
	if (IS_T(&(dcgettext), char *(*)(const char *, const char *, int, int)))
		pc_fail("dcgettext() is declared without a prototype");
	if (!IS_T(&(dcgettext_l), char *(*)(const char *, const char *, int, locale_t)))
		pc_fail("dcgettext_l() is not declared as char *dcgettext_l(const char *, const char *, int, locale_t)");
	if (IS_T(&(dcgettext_l), char *(*)(const char *, const char *, int, locale_t, int)))
		pc_fail("dcgettext_l() is declared without a prototype");
	if (!IS_T(&(dcngettext), char *(*)(const char *, const char *, const char *, unsigned long int, int)))
		pc_fail("dcngettext() is not declared as char *dcngettext(const char *, const char *, const char *, unsigned long int, int)");
	if (IS_T(&(dcngettext), char *(*)(const char *, const char *, const char *, unsigned long int, int, int)))
		pc_fail("dcngettext() is declared without a prototype");
	if (!IS_T(&(dcngettext_l), char *(*)(const char *, const char *, const char *, unsigned long int, int, locale_t)))
		pc_fail("dcngettext_l() is not declared as char *dcngettext_l(const char *, const char *, const char *, unsigned long int, int, locale_t)");
	if (IS_T(&(dcngettext_l), char *(*)(const char *, const char *, const char *, unsigned long int, int, locale_t, int)))
		pc_fail("dcngettext_l() is declared without a prototype");
	if (!IS_T(&(dgettext), char *(*)(const char *, const char *)))
		pc_fail("dgettext() is not declared as char *dgettext(const char *, const char *)");
	if (IS_T(&(dgettext), char *(*)(const char *, const char *, int)))
		pc_fail("dgettext() is declared without a prototype");
	if (!IS_T(&(dgettext_l), char *(*)(const char *, const char *, locale_t)))
		pc_fail("dgettext_l() is not declared as char *dgettext_l(const char *, const char *, locale_t)");
	if (IS_T(&(dgettext_l), char *(*)(const char *, const char *, locale_t, int)))
		pc_fail("dgettext_l() is declared without a prototype");
	if (!IS_T(&(dngettext), char *(*)(const char *, const char *, const char *, unsigned long int)))
		pc_fail("dngettext() is not declared as char *dngettext(const char *, const char *, const char *, unsigned long int)");
	if (IS_T(&(dngettext), char *(*)(const char *, const char *, const char *, unsigned long int, int)))
		pc_fail("dngettext() is declared without a prototype");
	if (!IS_T(&(dngettext_l), char *(*)(const char *, const char *, const char *, unsigned long int, locale_t)))
		pc_fail("dngettext_l() is not declared as char *dngettext_l(const char *, const char *, const char *, unsigned long int, locale_t)");
	if (IS_T(&(dngettext_l), char *(*)(const char *, const char *, const char *, unsigned long int, locale_t, int)))
		pc_fail("dngettext_l() is declared without a prototype");
	if (!IS_T(&(gettext), char *(*)(const char *)))
		pc_fail("gettext() is not declared as char *gettext(const char *)");
	if (IS_T(&(gettext), char *(*)(const char *, int)))
		pc_fail("gettext() is declared without a prototype");
	if (!IS_T(&(gettext_l), char *(*)(const char *, locale_t)))
		pc_fail("gettext_l() is not declared as char *gettext_l(const char *, locale_t)");
	if (IS_T(&(gettext_l), char *(*)(const char *, locale_t, int)))
		pc_fail("gettext_l() is declared without a prototype");
	if (!IS_T(&(ngettext), char *(*)(const char *, const char *, unsigned long int)))
		pc_fail("ngettext() is not declared as char *ngettext(const char *, const char *, unsigned long int)");
	if (IS_T(&(ngettext), char *(*)(const char *, const char *, unsigned long int, int)))
		pc_fail("ngettext() is declared without a prototype");
	if (!IS_T(&(ngettext_l), char *(*)(const char *, const char *, unsigned long int, locale_t)))
		pc_fail("ngettext_l() is not declared as char *ngettext_l(const char *, const char *, unsigned long int, locale_t)");
	if (IS_T(&(ngettext_l), char *(*)(const char *, const char *, unsigned long int, locale_t, int)))
		pc_fail("ngettext_l() is declared without a prototype");
	if (!IS_T(&(textdomain), char *(*)(const char *)))
		pc_fail("textdomain() is not declared as char *textdomain(const char *)");
	if (IS_T(&(textdomain), char *(*)(const char *, int)))
		pc_fail("textdomain() is declared without a prototype");
	pc_pass();
}
