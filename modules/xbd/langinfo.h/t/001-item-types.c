/*
 * The langinfo item constants have type nl_item.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.langinfo.h.001
 */
#include <langinfo.h>
#include "pc.h"

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

#define T(c) pc_check(IS_T(c, nl_item), #c " does not have type nl_item")

int main(void)
{
	pc_begin("XBD.langinfo.h.001");
	pc_test();
	T(CODESET);
	T(D_T_FMT);
	T(D_FMT);
	T(T_FMT);
	T(T_FMT_AMPM);
	T(AM_STR);
	T(PM_STR);
	T(DAY_1);
	T(DAY_2);
	T(DAY_3);
	T(DAY_4);
	T(DAY_5);
	T(DAY_6);
	T(DAY_7);
	T(ABDAY_1);
	T(ABDAY_2);
	T(ABDAY_3);
	T(ABDAY_4);
	T(ABDAY_5);
	T(ABDAY_6);
	T(ABDAY_7);
	T(MON_1);
	T(MON_2);
	T(MON_3);
	T(MON_4);
	T(MON_5);
	T(MON_6);
	T(MON_7);
	T(MON_8);
	T(MON_9);
	T(MON_10);
	T(MON_11);
	T(MON_12);
	T(ABMON_1);
	T(ABMON_2);
	T(ABMON_3);
	T(ABMON_4);
	T(ABMON_5);
	T(ABMON_6);
	T(ABMON_7);
	T(ABMON_8);
	T(ABMON_9);
	T(ABMON_10);
	T(ABMON_11);
	T(ABMON_12);
	T(ERA);
	T(ERA_D_FMT);
	T(ERA_D_T_FMT);
	T(ERA_T_FMT);
	T(ALT_DIGITS);
	T(RADIXCHAR);
	T(THOUSEP);
	T(YESEXPR);
	T(NOEXPR);
	T(CRNCYSTR);
	pc_pass();
}
