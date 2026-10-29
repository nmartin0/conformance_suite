/*
 * Compiled callers do not rely on the unspecified upper bits of a char or short return value.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION ABI.i386.calling.024
 */
#include "../../../../check.h"

#ifndef ABI_ARCH_I386
ABI_NOT_THIS_ARCH("ABI.i386.calling.024")
#else
signed char rsc(void);
unsigned char ruc(void);
short rs(void);
unsigned short rus(void);
char rc(void);
ABI_ASM_RET_EAX(rsc, 0x12345680);
ABI_ASM_RET_EAX(ruc, 0x123456F0);
ABI_ASM_RET_EAX(rs, 0x12348001);
ABI_ASM_RET_EAX(rus, 0x1234F001);
ABI_ASM_RET_EAX(rc, 0x12345680);
int main(void)
{
	pc_begin("ABI.i386.calling.024");
	int v;

	pc_test();
	v = rsc();
	pc_check(v == -128, "a signed char returned as 0x12345680 reads as %d, expected -128", v);
	v = ruc();
	pc_check(v == 0xF0, "an unsigned char returned as 0x123456F0 reads as %d, expected 240", v);
	v = rs();
	pc_check(v == -32767, "a short returned as 0x12348001 reads as %d, expected -32767", v);
	v = rus();
	pc_check(v == 0xF001, "an unsigned short returned as 0x1234F001 reads as %d, expected 61441", v);
	v = rc();
	pc_check(v == -128, "a char returned as 0x12345680 reads as %d, expected -128", v);
	pc_pass();
}
#endif
