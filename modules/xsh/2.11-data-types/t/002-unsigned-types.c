/*
 * ino_t, size_t, rlim_t and reclen_t are unsigned integer types.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.11-data-types.002
 */
#include "pc.h"
#include <dirent.h>
#include <stddef.h>
#include <sys/resource.h>
#include <sys/types.h>

#define OPS(T) ((T)((((T)1 << 1) | ((T)6 & (T)3)) ^ ~(T)0) >> 1)
#define UNSIGNED(T) ((T)~(T)0 > (T)0)

_Static_assert(UNSIGNED(ino_t), "ino_t is not unsigned");
_Static_assert(UNSIGNED(size_t), "size_t is not unsigned");
_Static_assert(UNSIGNED(rlim_t), "rlim_t is not unsigned");
_Static_assert(UNSIGNED(reclen_t), "reclen_t is not unsigned");

int main(void)
{
	volatile int v;

	pc_begin("XSH.2.11-data-types.002");
	pc_test();
	v = (OPS(ino_t) != 0) + (OPS(size_t) != 0) + (OPS(rlim_t) != 0) + (OPS(reclen_t) != 0);
	(void)v;
	pc_check(UNSIGNED(ino_t), "ino_t is not unsigned");
	pc_check(UNSIGNED(size_t), "size_t is not unsigned");
	pc_check(UNSIGNED(rlim_t), "rlim_t is not unsigned");
	pc_check(UNSIGNED(reclen_t), "reclen_t is not unsigned");
	pc_pass();
}
