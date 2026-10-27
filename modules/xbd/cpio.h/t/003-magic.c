/*
 * MAGIC is the string 070707.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.cpio.h.003
 */
#include <cpio.h>
#include <string.h>
#include "pc.h"

int main(void)
{
	pc_begin("XBD.cpio.h.003");
	pc_test();
#ifndef MAGIC
	pc_fail("<cpio.h> does not define MAGIC");
#else
	pc_check(strcmp(MAGIC, "070707") == 0, "MAGIC is \"%s\", not \"070707\"", MAGIC);
	pc_pass();
#endif
}
