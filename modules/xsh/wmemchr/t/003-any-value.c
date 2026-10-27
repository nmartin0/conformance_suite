/*
 * wmemchr() treats every wchar_t value alike.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wmemchr.003
 */
#include "pc.h"
#include <wchar.h>

static wchar_t w[] = { 1, WCHAR_MAX, 5 };

int main(void)
{
	wchar_t *r;

	pc_begin("XSH.wmemchr.003");
	pc_test();
	r = wmemchr(w, WCHAR_MAX, 3);
	pc_check(r == w + 1, "WCHAR_MAX was not found at index 1");
	pc_pass();
}
