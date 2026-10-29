/*
 * The ALTMON_ and ABALTMON_ item constants have values of their own.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.langinfo.h.004
 */
#include <langinfo.h>
#include "pc.h"

#if !defined(CODESET) || \
    !defined(D_T_FMT) || \
    !defined(D_FMT) || \
    !defined(T_FMT) || \
    !defined(T_FMT_AMPM) || \
    !defined(AM_STR) || \
    !defined(PM_STR) || \
    !defined(DAY_1) || \
    !defined(DAY_2) || \
    !defined(DAY_3) || \
    !defined(DAY_4) || \
    !defined(DAY_5) || \
    !defined(DAY_6) || \
    !defined(DAY_7) || \
    !defined(ABDAY_1) || \
    !defined(ABDAY_2) || \
    !defined(ABDAY_3) || \
    !defined(ABDAY_4) || \
    !defined(ABDAY_5) || \
    !defined(ABDAY_6) || \
    !defined(ABDAY_7) || \
    !defined(MON_1) || \
    !defined(MON_2) || \
    !defined(MON_3) || \
    !defined(MON_4) || \
    !defined(MON_5) || \
    !defined(MON_6) || \
    !defined(MON_7) || \
    !defined(MON_8) || \
    !defined(MON_9) || \
    !defined(MON_10) || \
    !defined(MON_11) || \
    !defined(MON_12) || \
    !defined(ABMON_1) || \
    !defined(ABMON_2) || \
    !defined(ABMON_3) || \
    !defined(ABMON_4) || \
    !defined(ABMON_5) || \
    !defined(ABMON_6) || \
    !defined(ABMON_7) || \
    !defined(ABMON_8) || \
    !defined(ABMON_9) || \
    !defined(ABMON_10) || \
    !defined(ABMON_11) || \
    !defined(ABMON_12) || \
    !defined(ERA) || \
    !defined(ERA_D_FMT) || \
    !defined(ERA_D_T_FMT) || \
    !defined(ERA_T_FMT) || \
    !defined(ALT_DIGITS) || \
    !defined(RADIXCHAR) || \
    !defined(THOUSEP) || \
    !defined(YESEXPR) || \
    !defined(NOEXPR) || \
    !defined(CRNCYSTR) || \
    !defined(ALTMON_1) || \
    !defined(ALTMON_2) || \
    !defined(ALTMON_3) || \
    !defined(ALTMON_4) || \
    !defined(ALTMON_5) || \
    !defined(ALTMON_6) || \
    !defined(ALTMON_7) || \
    !defined(ALTMON_8) || \
    !defined(ALTMON_9) || \
    !defined(ALTMON_10) || \
    !defined(ALTMON_11) || \
    !defined(ALTMON_12) || \
    !defined(ABALTMON_1) || \
    !defined(ABALTMON_2) || \
    !defined(ABALTMON_3) || \
    !defined(ABALTMON_4) || \
    !defined(ABALTMON_5) || \
    !defined(ABALTMON_6) || \
    !defined(ABALTMON_7) || \
    !defined(ABALTMON_8) || \
    !defined(ABALTMON_9) || \
    !defined(ABALTMON_10) || \
    !defined(ABALTMON_11) || \
    !defined(ABALTMON_12)
#define PC_MISSING 1
#endif

#ifndef PC_MISSING
static const struct { const char *name; nl_item value; } item[] = {
	{ "CODESET", CODESET },
	{ "D_T_FMT", D_T_FMT },
	{ "D_FMT", D_FMT },
	{ "T_FMT", T_FMT },
	{ "T_FMT_AMPM", T_FMT_AMPM },
	{ "AM_STR", AM_STR },
	{ "PM_STR", PM_STR },
	{ "DAY_1", DAY_1 },
	{ "DAY_2", DAY_2 },
	{ "DAY_3", DAY_3 },
	{ "DAY_4", DAY_4 },
	{ "DAY_5", DAY_5 },
	{ "DAY_6", DAY_6 },
	{ "DAY_7", DAY_7 },
	{ "ABDAY_1", ABDAY_1 },
	{ "ABDAY_2", ABDAY_2 },
	{ "ABDAY_3", ABDAY_3 },
	{ "ABDAY_4", ABDAY_4 },
	{ "ABDAY_5", ABDAY_5 },
	{ "ABDAY_6", ABDAY_6 },
	{ "ABDAY_7", ABDAY_7 },
	{ "MON_1", MON_1 },
	{ "MON_2", MON_2 },
	{ "MON_3", MON_3 },
	{ "MON_4", MON_4 },
	{ "MON_5", MON_5 },
	{ "MON_6", MON_6 },
	{ "MON_7", MON_7 },
	{ "MON_8", MON_8 },
	{ "MON_9", MON_9 },
	{ "MON_10", MON_10 },
	{ "MON_11", MON_11 },
	{ "MON_12", MON_12 },
	{ "ABMON_1", ABMON_1 },
	{ "ABMON_2", ABMON_2 },
	{ "ABMON_3", ABMON_3 },
	{ "ABMON_4", ABMON_4 },
	{ "ABMON_5", ABMON_5 },
	{ "ABMON_6", ABMON_6 },
	{ "ABMON_7", ABMON_7 },
	{ "ABMON_8", ABMON_8 },
	{ "ABMON_9", ABMON_9 },
	{ "ABMON_10", ABMON_10 },
	{ "ABMON_11", ABMON_11 },
	{ "ABMON_12", ABMON_12 },
	{ "ERA", ERA },
	{ "ERA_D_FMT", ERA_D_FMT },
	{ "ERA_D_T_FMT", ERA_D_T_FMT },
	{ "ERA_T_FMT", ERA_T_FMT },
	{ "ALT_DIGITS", ALT_DIGITS },
	{ "RADIXCHAR", RADIXCHAR },
	{ "THOUSEP", THOUSEP },
	{ "YESEXPR", YESEXPR },
	{ "NOEXPR", NOEXPR },
	{ "CRNCYSTR", CRNCYSTR },
	{ "ALTMON_1", ALTMON_1 },
	{ "ALTMON_2", ALTMON_2 },
	{ "ALTMON_3", ALTMON_3 },
	{ "ALTMON_4", ALTMON_4 },
	{ "ALTMON_5", ALTMON_5 },
	{ "ALTMON_6", ALTMON_6 },
	{ "ALTMON_7", ALTMON_7 },
	{ "ALTMON_8", ALTMON_8 },
	{ "ALTMON_9", ALTMON_9 },
	{ "ALTMON_10", ALTMON_10 },
	{ "ALTMON_11", ALTMON_11 },
	{ "ALTMON_12", ALTMON_12 },
	{ "ABALTMON_1", ABALTMON_1 },
	{ "ABALTMON_2", ABALTMON_2 },
	{ "ABALTMON_3", ABALTMON_3 },
	{ "ABALTMON_4", ABALTMON_4 },
	{ "ABALTMON_5", ABALTMON_5 },
	{ "ABALTMON_6", ABALTMON_6 },
	{ "ABALTMON_7", ABALTMON_7 },
	{ "ABALTMON_8", ABALTMON_8 },
	{ "ABALTMON_9", ABALTMON_9 },
	{ "ABALTMON_10", ABALTMON_10 },
	{ "ABALTMON_11", ABALTMON_11 },
	{ "ABALTMON_12", ABALTMON_12 },
};
#endif

int main(void)
{
#ifdef PC_MISSING
	pc_begin("XBD.langinfo.h.004");
	pc_unresolved("a langinfo item constant is missing (XBD.langinfo.h.001 and .003 report it)");
#else
	size_t i, j, n = sizeof item / sizeof item[0];

	pc_begin("XBD.langinfo.h.004");
	pc_test();
	for (i = 0; i < n; i++)
		for (j = i + 1; j < n; j++)
			pc_check(item[i].value != item[j].value, "%s and %s are both %ld", item[i].name, item[j].name, (long)item[i].value);
	pc_pass();
#endif
}
