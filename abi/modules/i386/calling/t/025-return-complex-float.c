/*
 * _Complex float comes back with the real part in EAX and the imaginary part in EDX.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION ABI.i386.calling.025
 */
#include "../../../../check.h"
#include <string.h>

#ifndef ABI_ARCH_I386
ABI_NOT_THIS_ARCH("ABI.i386.calling.025")
#else
__attribute__((noinline)) _Complex float f_cf(void)
{
	float p[2];
	_Complex float z;

	p[0] = 1.5f;
	p[1] = 2.5f;
	memcpy(&z, p, sizeof z);
	return z;
}
int main(void)
{
	pc_begin("ABI.i386.calling.025");
	struct abi_call c;

	pc_test();
	abi_call_init(&c, (void *)(size_t)&f_cf);
	abi_call(&c);
	pc_check(c.eax == 0x3FC00000u, "the real part 1.5f came back as 0x%08x in EAX", c.eax);
	pc_check(c.edx == 0x40200000u, "the imaginary part 2.5f came back as 0x%08x in EDX", c.edx);
	pc_check(c.npop == 0, "_Complex float left %u value(s) on the x87 register stack", c.npop);
	pc_pass();
}
#endif
