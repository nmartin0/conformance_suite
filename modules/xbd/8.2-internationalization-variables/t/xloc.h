/*
 * xloc.h - a locale X made with localedef that differs from the POSIX
 * locale in every category, and a way to tell which of the two a category
 * of the current locale behaves as.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * X: a character at byte 0xe1 is lower-case; b collates before a;
 * the currency symbol is "Q"; the radix is a comma; %a for Sunday is "ua";
 * the affirmative expression is ^([jJ]).
 */
#ifndef XLOC_H
#define XLOC_H

#include "tlib.h"
#include <ctype.h>
#include <langinfo.h>

static char xl_loc[4200];

static const int xl_cats[6] = { LC_COLLATE, LC_CTYPE, LC_MESSAGES, LC_MONETARY, LC_NUMERIC, LC_TIME };
static const char *const xl_names[6] = { "LC_COLLATE", "LC_CTYPE", "LC_MESSAGES", "LC_MONETARY", "LC_NUMERIC", "LC_TIME" };
static const char *const xl_vars[6] = { "LC_COLLATE", "LC_CTYPE", "LC_MESSAGES", "LC_MONETARY", "LC_NUMERIC", "LC_TIME" };

static void xl_make(void)
{
	static char text[30000], tm_[12000];
	char yes[100], no[100];
	size_t tlen;
	int i;

	pcc_init();
	for (i = 0; i < PCC_N; i++)
		if ((unsigned char)pcc_all[i] == 0xe1)
			pc_notinuse("a portable character is encoded at byte 0xe1");
	ld_charmap("xcmap", LD_HDR, '\\', "<x-low1> \\xe1\n", "", 0);
	tlen = tl_start(tm_, sizeof tm_);
	tlen = tl_formats(tm_, sizeof tm_, tlen);
	(void)tl_end(tm_, sizeof tm_, tlen);
	(void)snprintf(text, sizeof text,
	    "LC_CTYPE\nlower <x-low1>\nEND LC_CTYPE\n"
	    "LC_COLLATE\norder_start forward\n<b>\n<a>\nUNDEFINED\norder_end\nEND LC_COLLATE\n"
	    "LC_MONETARY\ncurrency_symbol \"<Q>\"\nEND LC_MONETARY\n"
	    "LC_NUMERIC\ndecimal_point \"<comma>\"\nthousands_sep \"<period>\"\ngrouping 3\nEND LC_NUMERIC\n"
	    "LC_MESSAGES\nyesexpr %s\nnoexpr %s\nEND LC_MESSAGES\n%s",
	    pcc_str(yes, sizeof yes, "^([jJ])"), pcc_str(no, sizeof no, "^([kK])"), tm_);
	ld_ok("xloc", "xcmap", text, xl_loc, sizeof xl_loc);
}

/* 1 if the category behaves as X, 0 if as the POSIX locale, -1 if as neither. */
static int xl_obs(int cat)
{
	char buf[100];
	struct tm t;
	const char *s;

	switch (cat) {
	case LC_NUMERIC:
		s = localeconv()->decimal_point;
		return strcmp(s, ",") == 0 ? 1 : strcmp(s, ".") == 0 ? 0 : -1;
	case LC_MONETARY:
		s = localeconv()->currency_symbol;
		return strcmp(s, "Q") == 0 ? 1 : s[0] == '\0' ? 0 : -1;
	case LC_TIME:
		t = tl_tm(0, 0);
		if (strftime(buf, sizeof buf, "%a", &t) == 0)
			return -1;
		return strcmp(buf, "ua") == 0 ? 1 : strcmp(buf, "Sun") == 0 ? 0 : -1;
	case LC_CTYPE:
		return islower(0xe1) ? 1 : 0;
	case LC_COLLATE:
		return strcoll("b", "a") < 0 ? 1 : 0;
	case LC_MESSAGES:
		s = nl_langinfo(YESEXPR);
		return s != NULL && strcmp(s, "^([jJ])") == 0 ? 1 : 0;
	default:
		return -1;
	}
}

/* Set the internationalization variables; NULL leaves a variable unset. per[] is in xl_cats order. */
static void xl_env(const char *lc_all, const char *lang, const char *const per[6])
{
	int i;

	ld_unset_all();
	if (lc_all != NULL && setenv("LC_ALL", lc_all, 1) != 0)
		pc_unresolved("setenv failed");
	if (lang != NULL && setenv("LANG", lang, 1) != 0)
		pc_unresolved("setenv failed");
	for (i = 0; i < 6; i++)
		if (per != NULL && per[i] != NULL && setenv(xl_vars[i], per[i], 1) != 0)
			pc_unresolved("setenv failed");
}

/* Initialise the locale from the environment. */
static void xl_select(void)
{
	if (setlocale(LC_ALL, "") == NULL)
		pc_unresolved("the locale named by the environment cannot be selected");
}

/* Check every category against want[]: 1 for locale X, 0 for the POSIX locale. */
static void xl_expect(const char *what, const int want[6])
{
	static const char *const nm[2] = { "the POSIX locale", "locale X" };
	int i, got;

	for (i = 0; i < 6; i++) {
		got = xl_obs(xl_cats[i]);
		pc_check(got == want[i], "%s: %s behaves as %s, expected %s", what, xl_names[i],
		    got == 0 || got == 1 ? nm[got] : "neither locale", nm[want[i]]);
	}
}

#endif
