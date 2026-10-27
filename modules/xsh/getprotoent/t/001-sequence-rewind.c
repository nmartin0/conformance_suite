/*
 * Reading the protocols database entry by entry after the open call ends with a null pointer, and opening it again gives the same sequence.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getprotoent.001
 */
#include "dbcommon.h"

int main(void)
{
	pc_begin("XSH.getprotoent.001");
	pc_test();
	collect(0, 0, 0);
	if (seqn[0] == 0)
		pc_notinuse("the database has no entry");
	collect(1, 0, 0);
	same_sequences();
	pc_pass();
}
