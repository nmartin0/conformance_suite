/*
 * argc is not negative and argv[argc] is a null pointer.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION ABI.i386.entry.003
 */
#include "../../../../check.h"

#ifndef ABI_ARCH_I386
ABI_NOT_THIS_ARCH("ABI.i386.entry.003")
#else
int main(int argc, char **argv)
{
	int i;

	pc_begin("ABI.i386.entry.003");
	pc_test();
	pc_check(argc >= 0, "argc is %d", argc);
	for (i = 0; i < argc; i++)
		pc_check(argv[i] != NULL, "argv[%d] is a null pointer (argc is %d)", i, argc);
	pc_check(argv[argc] == NULL, "argv[%d] is not a null pointer", argc);
	pc_pass();
}
#endif
