/*
 * A name from tmpnam() fits in L_tmpnam bytes.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.stdio.h.008
 */
#include <stdio.h>
#include <string.h>
#include "pc.h"

int main(void)
{
	pc_begin("XBD.stdio.h.008");
	pc_test();
	char buf[L_tmpnam];
	char *r = tmpnam(buf);

	if (r == NULL)
		pc_unresolved("tmpnam() returned a null pointer");
	pc_check(strlen(r) < L_tmpnam, "tmpnam() produced a name of %zu bytes; L_tmpnam is %d", strlen(r), (int)L_tmpnam);
	pc_pass();
}
