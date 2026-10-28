/*
 * <stdlib.h> functions adv
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.stdlib.h.010
 *
 * Only <stdlib.h> is included (not pc.h, which brings in <stddef.h>), so a name
 * the header fails to provide stops the build, which the module reports as FAIL.
 */
#include <stdlib.h>

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

int main(void)
{
	pc_begin("XBD.stdlib.h.010");
	pc_test();
	if (!IS_T(&(posix_memalign), int (*)(void **, size_t, size_t)))
		pc_fail("posix_memalign() is not declared as int posix_memalign(void **, size_t, size_t)");
	if (IS_T(&(posix_memalign), int (*)(void **, size_t, size_t, int)))
		pc_fail("posix_memalign() is declared without a prototype");
	pc_pass();
}
