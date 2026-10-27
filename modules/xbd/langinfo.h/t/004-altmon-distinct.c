/*
 * The ALTMON_ and ABALTMON_ item constants have values of their own.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.langinfo.h.004
 */
#include <langinfo.h>
#include "pc.h"

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

int main(void)
{
	size_t i, j, n = sizeof item / sizeof item[0];

	pc_begin("XBD.langinfo.h.004");
	pc_test();
	for (i = 0; i < n; i++)
		for (j = i + 1; j < n; j++)
			pc_check(item[i].value != item[j].value, "%s and %s are both %ld", item[i].name, item[j].name, (long)item[i].value);
	pc_pass();
}
