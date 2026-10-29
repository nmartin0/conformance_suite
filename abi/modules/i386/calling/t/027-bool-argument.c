/*
 * A _Bool passed as an argument has bits 1 to 7 of its low byte zero.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION ABI.i386.calling.027
 */
#include "../../../../check.h"

#ifndef ABI_ARCH_I386
ABI_NOT_THIS_ARCH("ABI.i386.calling.027")
#else
/* The callee sees the raw argument words; the caller is compiled code
 * calling through a pointer of a different prototype. */
__attribute__((noinline)) unsigned raw3(unsigned a, unsigned b, unsigned c)
{
	return (a & 0xFFu) | ((b & 0xFFu) << 8) | ((c & 0xFFu) << 16);
}
static unsigned (*volatile fp)(_Bool, _Bool, _Bool) = (unsigned (*)(_Bool, _Bool, _Bool))(void *)raw3;
static volatile int vals[3] = { 77, 2, 0 };

int main(void)
{
	pc_begin("ABI.i386.calling.027");
	unsigned r;
	int k;

	pc_test();
	r = fp(vals[0] > 3, vals[1] > 3, vals[2] > 3);
	pc_check((r & 0xFFu) == 1, "a true _Bool argument had low byte 0x%02x", r & 0xFFu);
	pc_check(((r >> 8) & 0xFFu) == 0, "a false _Bool argument had low byte 0x%02x", (r >> 8) & 0xFFu);
	pc_check(((r >> 16) & 0xFFu) == 0, "a false _Bool argument had low byte 0x%02x", (r >> 16) & 0xFFu);
	for (k = 0; k < 3; k++)
		vals[k] = 256 + 2 * k;	/* nonzero values whose low byte is even */
	r = fp(vals[0], vals[1], vals[2]);
	pc_check((r & 0xFFu) == 1, "a _Bool converted from 256 had low byte 0x%02x", r & 0xFFu);
	pc_check(((r >> 8) & 0xFFu) == 1, "a _Bool converted from 258 had low byte 0x%02x", (r >> 8) & 0xFFu);
	pc_check(((r >> 16) & 0xFFu) == 1, "a _Bool converted from 260 had low byte 0x%02x", (r >> 16) & 0xFFu);
	pc_pass();
}
#endif
