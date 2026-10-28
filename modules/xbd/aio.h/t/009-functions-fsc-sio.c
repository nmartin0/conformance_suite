/*
 * <aio.h> functions fsc sio
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.aio.h.009
 *
 * Only <aio.h> is included (not pc.h, which brings in <stddef.h>), so a name
 * the header fails to provide stops the build, which the module reports as FAIL.
 */
#include <aio.h>

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

int main(void)
{
	pc_begin("XBD.aio.h.009");
	pc_test();
	if (!IS_T(&(aio_fsync), int (*)(int, struct aiocb *)))
		pc_fail("aio_fsync() is not declared as int aio_fsync(int, struct aiocb *)");
	if (IS_T(&(aio_fsync), int (*)(int, struct aiocb *, int)))
		pc_fail("aio_fsync() is declared without a prototype");
	pc_pass();
}
