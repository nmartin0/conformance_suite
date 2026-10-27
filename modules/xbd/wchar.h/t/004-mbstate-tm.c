/*
 * mbstate_t is a complete object type that is not an array; struct tm is
 *   an incomplete tag.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.wchar.h.004
 */
#include <wchar.h>
#include <string.h>
#include "pc.h"


int main(void)
{
	mbstate_t a, b;
	struct tm *tp = NULL;

	pc_begin("XBD.wchar.h.004");
	pc_test();
	pc_check(sizeof a > 0, "mbstate_t is not a complete type");
	memset(&a, 0, sizeof a);
	b = a;
	pc_check(memcmp(&a, &b, sizeof a) == 0, "an mbstate_t was not copied by assignment");
	pc_check(tp == NULL, "a null pointer to struct tm is not null");
	pc_pass();
}
