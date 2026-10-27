/*
 * btowc(EOF) is WEOF.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.btowc.002
 */
#include "pc.h"
#include <stdio.h>
#include <wchar.h>

int main(void)
{
	wint_t r;

	pc_begin("XSH.btowc.002");
	pc_test();
	r = btowc(EOF);
	pc_check(r == WEOF, "btowc(EOF) returned %lu, expected WEOF", (unsigned long)r);
	pc_pass();
}
