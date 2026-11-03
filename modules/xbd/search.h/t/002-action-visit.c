/*
 * <search.h> ACTION and VISIT
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.search.h.002
 *
 * Only <search.h> is included (not pc.h, which brings in <stddef.h>), so a name
 * the header fails to provide stops the build, which the module reports as FAIL.
 */
#include <search.h>

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

int main(void)
{
	pc_begin("XBD.search.h.002");
	pc_test();
	ACTION a[2];
	VISIT v[4];
	int i, j;

	a[0] = FIND;
	a[1] = ENTER;
	v[0] = preorder;
	v[1] = postorder;
	v[2] = endorder;
	v[3] = leaf;
	for (i = 0; i < 2; i++)
		for (j = i + 1; j < 2; j++)
			if ((int)a[i] == (int)a[j])
				pc_fail("two ACTION constants have the same value %d", (int)a[i]);
	for (i = 0; i < 4; i++)
		for (j = i + 1; j < 4; j++)
			if ((int)v[i] == (int)v[j])
				pc_fail("two VISIT constants have the same value %d", (int)v[i]);
	/* constant expressions: duplicate case values would stop the build */
	switch (a[0]) {
	case FIND:
	case ENTER:
		break;
	}
	switch (v[0]) {
	case preorder:
	case postorder:
	case endorder:
	case leaf:
		break;
	}
	pc_pass();
}
