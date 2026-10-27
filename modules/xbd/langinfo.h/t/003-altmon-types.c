/*
 * The ALTMON_ and ABALTMON_ item constants have type nl_item.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.langinfo.h.003
 */
#include <langinfo.h>
#include "pc.h"

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

#define T(c) pc_check(IS_T(c, nl_item), #c " does not have type nl_item")

int main(void)
{
	pc_begin("XBD.langinfo.h.003");
	pc_test();
	T(ALTMON_1);
	T(ALTMON_2);
	T(ALTMON_3);
	T(ALTMON_4);
	T(ALTMON_5);
	T(ALTMON_6);
	T(ALTMON_7);
	T(ALTMON_8);
	T(ALTMON_9);
	T(ALTMON_10);
	T(ALTMON_11);
	T(ALTMON_12);
	T(ABALTMON_1);
	T(ABALTMON_2);
	T(ABALTMON_3);
	T(ABALTMON_4);
	T(ABALTMON_5);
	T(ABALTMON_6);
	T(ABALTMON_7);
	T(ABALTMON_8);
	T(ABALTMON_9);
	T(ABALTMON_10);
	T(ABALTMON_11);
	T(ABALTMON_12);
	pc_pass();
}
