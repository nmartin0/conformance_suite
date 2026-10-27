/*
 * A name from ctermid() fits in L_ctermid bytes.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.stdio.h.009
 */
#include <stdio.h>
#include <string.h>
#include "pc.h"

int main(void)
{
	pc_begin("XBD.stdio.h.009");
	pc_test();
	char buf[L_ctermid];
	char *r;

	memset(buf, 'x', sizeof buf);
	r = ctermid(buf);
	pc_check(r != NULL, "ctermid() returned a null pointer");
	pc_check(memchr(buf, '\0', sizeof buf) != NULL, "ctermid() did not end its result inside an array of L_ctermid bytes");
	pc_pass();
}
