/*
 * With a non-zero argument to the open call, the networks database is read through to the same sequence of entries.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getnetent.008
 */
#include "dbcommon.h"

int main(void)
{
	pc_begin("XSH.getnetent.008");
	pc_test();
	collect(0, 0, 0);
	if (seqn[0] == 0)
		pc_notinuse("the database has no entry");
	collect(1, 1, 0);
	ENDENT();
	same_sequences();
	pc_pass();
}
