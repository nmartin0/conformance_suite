/*
 * The program's own file identifies itself as a 32-bit, little-endian file
 * for the i386 architecture with no processor flags.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION ABI.i386.entry.015
 */
#include "../../../../check.h"
#include <stdio.h>

#ifndef ABI_ARCH_I386
ABI_NOT_THIS_ARCH("ABI.i386.entry.015")
#else
int main(int argc, char **argv)
{
	unsigned char h[52];
	FILE *f;
	size_t n;
	unsigned machine, flags;

	pc_begin("ABI.i386.entry.015");
	pc_test();
	(void)argc;
	if (argv[0] == NULL || argv[0][0] != '/')
		pc_unresolved("argv[0] is not an absolute path, the program's file cannot be found");
	f = fopen(argv[0], "rb");
	if (f == NULL)
		pc_unresolved("cannot open the program file named by argv[0]");
	n = fread(h, 1, sizeof h, f);
	(void)fclose(f);
	if (n != sizeof h)
		pc_unresolved("the program file is shorter than a file header");
	/* The file header: identification bytes, then e_machine at 18 and e_flags at 36. */
	pc_check(h[0] == 0x7f && h[1] == 'E' && h[2] == 'L' && h[3] == 'F', "the program file does not start with the object file magic");
	pc_check(h[4] == 1, "the file class byte is %u, 32-bit is 1", h[4]);
	pc_check(h[5] == 1, "the data encoding byte is %u, little-endian is 1", h[5]);
	machine = h[18] | (unsigned)h[19] << 8;
	flags = h[36] | (unsigned)h[37] << 8 | (unsigned)h[38] << 16 | (unsigned)h[39] << 24;
	pc_check(machine == 3, "the machine field is %u, the i386 value is 3", machine);
	pc_check(flags == 0, "the processor flags are 0x%x, the i386 defines none", flags);
	pc_pass();
}
#endif
