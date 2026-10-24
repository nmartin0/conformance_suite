/*
 * <unistd.h> defines SEEK_SET, SEEK_CUR, SEEK_END, SEEK_HOLE and SEEK_DATA
 * as macros with integer constant values, all different.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.unistd.h.002
 */
#include "pc.h"
#include <unistd.h>

int main(void)
{
	pc_begin("XBD.unistd.h.002");
	pc_test();
#if !defined(SEEK_SET) || !defined(SEEK_CUR) || !defined(SEEK_END)
	pc_unresolved("SEEK_SET, SEEK_CUR or SEEK_END missing (XBD.unistd.h.003)");
#elif !defined(SEEK_HOLE) || !defined(SEEK_DATA)
	pc_fail("<unistd.h> does not define SEEK_HOLE and SEEK_DATA as macros");
#else
	{
		/* Integer constant expressions: usable as enumeration values. */
		_Static_assert((SEEK_SET) == (SEEK_SET), "SEEK_SET is an integer constant expression");
		_Static_assert((SEEK_CUR) == (SEEK_CUR), "SEEK_CUR is an integer constant expression");
		_Static_assert((SEEK_END) == (SEEK_END), "SEEK_END is an integer constant expression");
		_Static_assert((SEEK_HOLE) == (SEEK_HOLE), "SEEK_HOLE is an integer constant expression");
		_Static_assert((SEEK_DATA) == (SEEK_DATA), "SEEK_DATA is an integer constant expression");
#define S (SEEK_SET)
#define C (SEEK_CUR)
#define E (SEEK_END)
#define H (SEEK_HOLE)
#define D (SEEK_DATA)
		int v[5] = { S, C, E, H, D }, i, j;
		for (i = 0; i < 5; i++)
			for (j = i + 1; j < 5; j++)
				pc_check(v[i] != v[j], "two SEEK_* values are equal (%d)", v[i]);
	}
	pc_pass();
#endif
}
