/*
 * With a null ps mbrlen() keeps its own state between calls.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mbrlen.007
 */
#include "pc.h"
#include <locale.h>
#include <wchar.h>

static void use_utf8(void)
{
	const char *n = pc_find_locale("UTF-8", 1);

	if (n == NULL)
		pc_notinuse("no UTF-8 locale installed");
	if (setlocale(LC_ALL, n) == NULL)
		pc_unresolved("setlocale(LC_ALL, \"%s\") failed", n);
}

int main(void)
{
	size_t r1, r2;

	pc_begin("XSH.mbrlen.007");
	use_utf8();
	pc_test();
	r1 = mbrlen("\xC3", 1, NULL);
	r2 = mbrlen("\xA9", 1, NULL);
	pc_check(r1 == (size_t)-2, "mbrlen() of the first byte of a two-byte character returned %zu, expected (size_t)-2", r1);
	pc_check(r2 == 1, "mbrlen() of its last byte returned %zu, expected 1", r2);
	pc_pass();
}
