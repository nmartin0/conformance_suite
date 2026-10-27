/*
 * Every networks entry has the members the header names filled in: a name, and alias lists ending with a null pointer.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getnetent.002
 */
#include "dbcommon.h"

int main(void)
{
	pc_begin("XSH.getnetent.002");
	pc_test();
	collect(0, 0, 1);
	if (seqn[0] == 0)
		pc_notinuse("the database has no entry");
	pc_pass();
}
