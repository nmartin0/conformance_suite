/*
 * mbrlen() with a null ps uses its own state and works in the POSIX locale.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mbrlen.002
 */
#include "pc.h"
#include <wchar.h>

int main(void)
{
	size_t r1, r0;

	pc_begin("XSH.mbrlen.002");
	pc_test();
	r1 = mbrlen("A", 1, NULL);
	r0 = mbrlen("", 1, NULL);
	pc_check(r1 == 1, "mbrlen(\"A\", 1, NULL) returned %zu, expected 1", r1);
	pc_check(r0 == 0, "mbrlen(\"\", 1, NULL) returned %zu, expected 0", r0);
	pc_pass();
}
