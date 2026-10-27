/*
 * The state is wholly in the object ps points to: an unfinished character
 * in one object is not disturbed by conversion through another.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mbrtowc.010
 */
#include "pc.h"
#include <locale.h>
#include <string.h>
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
	mbstate_t a, b;
	wchar_t whole = 0, wa = 77, wb = 77;
	size_t n, ra1, rb, ra2;

	pc_begin("XSH.mbrtowc.010");
	use_utf8();
	memset(&a, 0, sizeof a);
	memset(&b, 0, sizeof b);
	n = mbrtowc(&whole, "\xC3\xA9", 2, &a);
	pc_check(n == 2, "setup: mbrtowc() of a two-byte character returned %zu", n);
	memset(&a, 0, sizeof a);
	pc_test();
	ra1 = mbrtowc(&wa, "\xC3", 1, &a);
	rb = mbrtowc(&wb, "\xE2\x82\xAC", 3, &b);
	ra2 = mbrtowc(&wa, "\xA9", 1, &a);
	pc_check(ra1 == (size_t)-2, "mbrtowc() on the first object returned %zu, expected (size_t)-2", ra1);
	pc_check(rb == 3, "mbrtowc() through the second object returned %zu, expected 3", rb);
	pc_check(ra2 == 1 && wa == whole, "mbrtowc() completing the character in the first object returned %zu or stored another code", ra2);
	pc_pass();
}
