/*
 * lc.h - helpers shared by the tests of one locale module.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * lc_init() must be the first thing a test does after pc_begin(): it records
 * the code set of the locale the program starts in (the POSIX locale). The
 * POSIX locale's values below are those of XBD 7.3.3.1, 7.3.4.1, 7.3.5.3 and
 * 7.3.6.1; its code set is not fixed by the standard, hence lc_c0.
 * A second locale, when the system has one, is found with pc_find_locale().
 */
#ifndef LC_H
#define LC_H

#include "pc.h"
#include <errno.h>
#include <langinfo.h>
#include <limits.h>
#include <locale.h>
#include <monetary.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LC_NOSUCH "pc_no_such_locale_x7q"

static char lc_c0[128];		/* code set of the starting (POSIX) locale */
static char lc_loc[256];	/* name of a second locale, "" if none */

static const int lc_cat[6] = { LC_COLLATE, LC_CTYPE, LC_MESSAGES, LC_MONETARY, LC_NUMERIC, LC_TIME };
static const int lc_mask[6] = { LC_COLLATE_MASK, LC_CTYPE_MASK, LC_MESSAGES_MASK, LC_MONETARY_MASK, LC_NUMERIC_MASK, LC_TIME_MASK };

static const nl_item lc_day[7] = { DAY_1, DAY_2, DAY_3, DAY_4, DAY_5, DAY_6, DAY_7 };
static const nl_item lc_abday[7] = { ABDAY_1, ABDAY_2, ABDAY_3, ABDAY_4, ABDAY_5, ABDAY_6, ABDAY_7 };
static const nl_item lc_mon[12] = { MON_1, MON_2, MON_3, MON_4, MON_5, MON_6, MON_7, MON_8, MON_9, MON_10, MON_11, MON_12 };
static const nl_item lc_abmon[12] = { ABMON_1, ABMON_2, ABMON_3, ABMON_4, ABMON_5, ABMON_6, ABMON_7, ABMON_8, ABMON_9, ABMON_10, ABMON_11, ABMON_12 };
static const char *const lc_dayname[7] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };
static const char *const lc_abdayname[7] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
static const char *const lc_monname[12] = { "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December" };
static const char *const lc_abmonname[12] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };

static inline void lc_cp(char *d, size_t n, const char *s)
{
	if (s == NULL) {
		d[0] = '\0';
		return;
	}
	strncpy(d, s, n - 1);
	d[n - 1] = '\0';
}

static inline void lc_init(void)
{
	lc_cp(lc_c0, sizeof lc_c0, nl_langinfo(CODESET));
	lc_loc[0] = '\0';
}

/* 1 and lc_loc set if the system has a second locale. Leaves the global locale at "C". */
static inline int lc_find(void)
{
	const char *n = pc_find_locale("UTF-8", 0);

	if (n == NULL) {
		lc_loc[0] = '\0';
		return 0;
	}
	lc_cp(lc_loc, sizeof lc_loc, n);
	return 1;
}

/* For tests that mean nothing without a second locale. */
static inline void lc_need(void)
{
	if (!lc_find())
		pc_notinuse("no installed locale with the UTF-8 code set was found");
	if (strcmp(lc_c0, "UTF-8") == 0)
		pc_unresolved("the POSIX locale reports the same code set as the second locale");
}

/* A name no locale has must be refused by setlocale(); if not, UNRESOLVED. */
static inline void lc_nosuch(void)
{
	if (setlocale(LC_ALL, LC_NOSUCH) != NULL) {
		(void)setlocale(LC_ALL, "C");
		pc_unresolved("the system accepts the made-up locale name " LC_NOSUCH);
	}
}

static inline void lc_env_clear(void)
{
	static const char *const v[] = { "LC_ALL", "LC_COLLATE", "LC_CTYPE", "LC_MESSAGES", "LC_MONETARY", "LC_NUMERIC", "LC_TIME", "LANG" };
	size_t i;

	for (i = 0; i < sizeof v / sizeof v[0]; i++)
		(void)unsetenv(v[i]);
}

static inline void lc_env_set(const char *name, const char *value)
{
	if (setenv(name, value, 1) != 0)
		pc_unresolved("setenv(%s) failed", name);
}

/* nl_langinfo() when o is (locale_t)0, else nl_langinfo_l() on o. */
static inline const char *lc_item(locale_t o, nl_item it)
{
	return o == (locale_t)0 ? nl_langinfo(it) : nl_langinfo_l(it, o);
}

/* Check the POSIX values of the time, numeric, messages items and the code set. */
static inline void lc_check_items(locale_t o, const char *w)
{
	int i;

	for (i = 0; i < 7; i++) {
		pc_check(strcmp(lc_item(o, lc_day[i]), lc_dayname[i]) == 0, "%s: DAY_%d is \"%s\", expected \"%s\"", w, i + 1, lc_item(o, lc_day[i]), lc_dayname[i]);
		pc_check(strcmp(lc_item(o, lc_abday[i]), lc_abdayname[i]) == 0, "%s: ABDAY_%d is \"%s\", expected \"%s\"", w, i + 1, lc_item(o, lc_abday[i]), lc_abdayname[i]);
	}
	for (i = 0; i < 12; i++) {
		pc_check(strcmp(lc_item(o, lc_mon[i]), lc_monname[i]) == 0, "%s: MON_%d is \"%s\", expected \"%s\"", w, i + 1, lc_item(o, lc_mon[i]), lc_monname[i]);
		pc_check(strcmp(lc_item(o, lc_abmon[i]), lc_abmonname[i]) == 0, "%s: ABMON_%d is \"%s\", expected \"%s\"", w, i + 1, lc_item(o, lc_abmon[i]), lc_abmonname[i]);
	}
	pc_check(strcmp(lc_item(o, AM_STR), "AM") == 0, "%s: AM_STR is \"%s\"", w, lc_item(o, AM_STR));
	pc_check(strcmp(lc_item(o, PM_STR), "PM") == 0, "%s: PM_STR is \"%s\"", w, lc_item(o, PM_STR));
	pc_check(strcmp(lc_item(o, D_T_FMT), "%a %b %e %H:%M:%S %Y") == 0, "%s: D_T_FMT is \"%s\"", w, lc_item(o, D_T_FMT));
	pc_check(strcmp(lc_item(o, D_FMT), "%m/%d/%y") == 0, "%s: D_FMT is \"%s\"", w, lc_item(o, D_FMT));
	pc_check(strcmp(lc_item(o, T_FMT), "%H:%M:%S") == 0, "%s: T_FMT is \"%s\"", w, lc_item(o, T_FMT));
	pc_check(strcmp(lc_item(o, T_FMT_AMPM), "%I:%M:%S %p") == 0, "%s: T_FMT_AMPM is \"%s\"", w, lc_item(o, T_FMT_AMPM));
	pc_check(strcmp(lc_item(o, RADIXCHAR), ".") == 0, "%s: RADIXCHAR is \"%s\"", w, lc_item(o, RADIXCHAR));
	pc_check(strcmp(lc_item(o, YESEXPR), "^[yY]") == 0, "%s: YESEXPR is \"%s\"", w, lc_item(o, YESEXPR));
	pc_check(strcmp(lc_item(o, NOEXPR), "^[nN]") == 0, "%s: NOEXPR is \"%s\"", w, lc_item(o, NOEXPR));
	pc_check(strcmp(lc_item(o, CODESET), lc_c0) == 0, "%s: CODESET is \"%s\", expected the POSIX locale's \"%s\"", w, lc_item(o, CODESET), lc_c0);
}

/*
 * Check that one category (or LC_ALL) of the current locale (o is (locale_t)0)
 * or of locale object o behaves as the POSIX locale. A probe per category
 * that the standard fixes: code set (LC_CTYPE), day and month names and
 * formats (LC_TIME), RADIXCHAR (LC_NUMERIC), YESEXPR and NOEXPR
 * (LC_MESSAGES), the order of "B" and "a" (LC_COLLATE) and, for the current
 * locale only, localeconv() (LC_MONETARY).
 */
static inline void lc_check_cat(locale_t o, int cat, const char *w)
{
	int i, c;

	if (cat == LC_ALL) {
		for (i = 0; i < 6; i++)
			lc_check_cat(o, lc_cat[i], w);
		return;
	}
	switch (cat) {
	case LC_CTYPE:
		pc_check(strcmp(lc_item(o, CODESET), lc_c0) == 0, "%s: CODESET is \"%s\", expected the POSIX locale's \"%s\"", w, lc_item(o, CODESET), lc_c0);
		break;
	case LC_TIME:
		pc_check(strcmp(lc_item(o, DAY_1), "Sunday") == 0 && strcmp(lc_item(o, ABMON_12), "Dec") == 0 &&
		    strcmp(lc_item(o, D_T_FMT), "%a %b %e %H:%M:%S %Y") == 0, "%s: the time items are not the POSIX locale's (DAY_1 \"%s\")", w, lc_item(o, DAY_1));
		break;
	case LC_NUMERIC:
		pc_check(strcmp(lc_item(o, RADIXCHAR), ".") == 0, "%s: RADIXCHAR is \"%s\"", w, lc_item(o, RADIXCHAR));
		break;
	case LC_MESSAGES:
		pc_check(strcmp(lc_item(o, YESEXPR), "^[yY]") == 0 && strcmp(lc_item(o, NOEXPR), "^[nN]") == 0, "%s: YESEXPR is \"%s\", NOEXPR \"%s\"", w, lc_item(o, YESEXPR), lc_item(o, NOEXPR));
		break;
	case LC_COLLATE:
		c = o == (locale_t)0 ? strcoll("B", "a") : strcoll_l("B", "a", o);
		pc_check(c < 0, "%s: strcoll(\"B\", \"a\") gave %d, expected negative", w, c);
		break;
	case LC_MONETARY:
		if (o == (locale_t)0) {
			const struct lconv *lc = localeconv();

			pc_check(lc->p_sign_posn == CHAR_MAX && lc->currency_symbol[0] == '\0' && lc->positive_sign[0] == '\0',
			    "%s: localeconv() does not show the POSIX locale's monetary values", w);
		}
		break;
	default:
		break;
	}
}

/* Query the global locale's name for a category into d. */
static inline void lc_q(int cat, char *d, size_t n)
{
	const char *q = setlocale(cat, NULL);

	pc_check(q != NULL, "setlocale(%d, NULL) returned a null pointer", cat);
	lc_cp(d, n, q);
}

#endif
