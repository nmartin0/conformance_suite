/*
 * In the POSIX locale btowc() accepts every byte value 0 to 255.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.btowc.001
 */
#include "pc.h"
#include <stdio.h>
#include <wchar.h>

int main(void)
{
	int c, bad = -1;

	pc_begin("XSH.btowc.001");
	pc_test();
	for (c = 0; c <= 255 && bad < 0; c++)
		if (btowc(c) == WEOF)
			bad = c;
	pc_check(bad < 0, "btowc(%d) returned WEOF in the POSIX locale", bad);
	pc_pass();
}
