/*
 * <semaphore.h> macros open flags
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.semaphore.h.004
 *
 * Only <semaphore.h> is included (not pc.h, which brings in <stddef.h>), so a name
 * the header fails to provide stops the build, which the module reports as FAIL.
 */
#include <semaphore.h>

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

int main(void)
{
	pc_begin("XBD.semaphore.h.004");
	pc_test();
#ifndef O_CREAT
	pc_fail("<semaphore.h> does not define O_CREAT");
#endif
#ifndef O_EXCL
	pc_fail("<semaphore.h> does not define O_EXCL");
#endif
	pc_pass();
}
