/*
 * memmem: empty needle.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.memmem.003
 */
#include "pc.h"
#include <string.h>
static void *(*volatile f_memmem)(const void *, size_t, const void *, size_t) = memmem;

int main(void)
{
	static const char h[4] = "abc";
	pc_begin("XSH.memmem.003");
	pc_test();
	pc_check(f_memmem(h, 4, h, 0) == h, "an empty needle did not give the haystack");
	pc_check(f_memmem(h, 0, h, 0) == h, "an empty needle in an empty haystack did not give the haystack");
	pc_check(f_memmem(h + 1, 3, "zzz", 0) == h + 1, "an empty needle did not give an inner haystack pointer");
	pc_pass();
}
