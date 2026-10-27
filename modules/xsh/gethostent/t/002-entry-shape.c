/*
 * Every host entry has a name, alias and address lists ending with a null pointer, and an address length matching its type.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.gethostent.002
 */
#include "dbcommon.h"

int main(void)
{
	pc_begin("XSH.gethostent.002");
	pc_test();
	collect(0, 0, 1);
	if (seqn[0] == 0)
		pc_notinuse("the database has no entry");
	pc_pass();
}
