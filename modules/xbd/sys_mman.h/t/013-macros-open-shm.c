/*
 * <sys/mman.h> macros open shm
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sys_mman.h.013
 *
 * Only <sys/mman.h> is included (not pc.h, which brings in <stddef.h>), so a name
 * the header fails to provide stops the build, which the module reports as FAIL.
 */
#include <sys/mman.h>

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

int main(void)
{
	pc_begin("XBD.sys_mman.h.013");
	pc_test();
#ifndef O_CREAT
	pc_fail("<sys/mman.h> does not define O_CREAT");
#endif
#ifndef O_EXCL
	pc_fail("<sys/mman.h> does not define O_EXCL");
#endif
#ifndef O_TRUNC
	pc_fail("<sys/mman.h> does not define O_TRUNC");
#endif
	pc_pass();
}
