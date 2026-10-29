/*
 * Object and function pointers are 4 bytes, aligned to 4; a null pointer is all zero bits.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION ABI.i386.types.003
 */
#include "../../../../check.h"
#include <string.h>

#ifndef ABI_ARCH_I386
ABI_NOT_THIS_ARCH("ABI.i386.types.003")
#else
struct s { int x; };
int main(void)
{
	pc_begin("ABI.i386.types.003");
	pc_test();
	_Static_assert(sizeof(void *) == 4 && _Alignof(void *) == 4, "void * must be 4 bytes, alignment 4");
	_Static_assert(sizeof(char *) == 4 && _Alignof(char *) == 4, "char * must be 4 bytes, alignment 4");
	_Static_assert(sizeof(struct s *) == 4 && _Alignof(struct s *) == 4, "a structure pointer must be 4 bytes, alignment 4");
	_Static_assert(sizeof(double *) == 4 && _Alignof(double *) == 4, "double * must be 4 bytes, alignment 4");
	_Static_assert(sizeof(void (*)(void)) == 4 && _Alignof(void (*)(void)) == 4, "a function pointer must be 4 bytes, alignment 4");
	_Static_assert(sizeof(int (*)()) == 4 && _Alignof(int (*)()) == 4, "an unprototyped function pointer must be 4 bytes, alignment 4");
	{
		void *vp = 0;
		char *cp = 0;
		void (*fp)(void) = 0;
		unsigned w;

		memcpy(&w, &vp, sizeof w);
		pc_check(w == 0, "a null object pointer has the bits 0x%08x", w);
		memcpy(&w, &cp, sizeof w);
		pc_check(w == 0, "a null char pointer has the bits 0x%08x", w);
		memcpy(&w, &fp, sizeof w);
		pc_check(w == 0, "a null function pointer has the bits 0x%08x", w);
	}
	pc_pass();
}
#endif
